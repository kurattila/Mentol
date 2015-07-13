#include "MainViewModel.h"

#include "src/MentolLib/ICalcResolver.h"
#include "src/MentolLib/ICalcOperationsVisualizer.h"
#include "src/MentolLib/KeypadProcessor.h"



MainViewModel::MainViewModel(QObject *parent)
    : QObject(parent)
    , m_ComplexityToPrecision_Model(new ComplexityToPrecision_Model())
    , m_KeypadShown(true)
{
    auto processor = new KeypadProcessor([this](){ hideKeypad(); }
                                       , [this](){ hideKeypad(); restartCalculation(); }
                                       , [this](){ emit keypadProcessorWholeInputChanged(); });
    m_KeypadProcessor.reset(processor);
}

void MainViewModel::calculate(float destValue)
{
    m_ComplexityToPrecision_Model->SetDestValue(destValue);
    emit complexityDistributionChanged();

    setUserPreferenceOfComplexity(15);
}

void MainViewModel::setUserPreferenceOfComplexity(int userPreferredtolerancePercents)
{
    std::list< std::pair<int, int> > complexityDistribution = m_ComplexityToPrecision_Model->GetCurrentComplexityDistribution();
    if (complexityDistribution.empty())
        return;

    int bestCandidateFromDistribution = complexityDistribution.front().second;
    int indexOfBestCandidateFromDistribution = complexityDistribution.size();
    for (auto it = complexityDistribution.rbegin(); it != complexityDistribution.rend(); ++it)
    {
        std::pair<int, int>& currentPrecision = *it;

        bestCandidateFromDistribution = currentPrecision.second;
        --indexOfBestCandidateFromDistribution;

        if (currentPrecision.second >= 100 - userPreferredtolerancePercents)
            break;
    }

    updateOperationsVisualization(100 - bestCandidateFromDistribution);
    m_ComplexityToPrecision_Model->SetUserPreferredIndexOfComplexity(indexOfBestCandidateFromDistribution);
}

void MainViewModel::keypadButtonPressed(QString buttonText)
{
    m_KeypadProcessor->OnKeyPressed(buttonText);
}

const QString &MainViewModel::getTextualVisualization() const
{
    return m_TextualVisualization;
}

ComplexityToPrecision_Model *MainViewModel::getComplexityDistribution()
{
    return m_ComplexityToPrecision_Model.get();
}

void MainViewModel::setKeypadShown(bool shown)
{
    m_KeypadShown = shown;
    emit keypadShownChanged();
}

QString MainViewModel::getKeypadProcessorWholeInput() const
{
    return m_KeypadProcessor->GetWholeInput();
}

void MainViewModel::updateOperationsVisualization(int tolerancePercents)
{
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue( m_ComplexityToPrecision_Model->GetDestValue() );
    resolver->SetTolerancePercents(tolerancePercents);

    auto operations = resolver->GetNeededOperations();

    std::unique_ptr<ICalcOperationsVisualizer> textualVisualizer( CreateCalcOperationsTextualVisualizer() );
    m_TextualVisualization = textualVisualizer->GetResult(operations);
    emit textualVisualizationChanged();
}

void MainViewModel::hideKeypad()
{
    m_KeypadShown = false;
    emit keypadShownChanged();
}

void MainViewModel::restartCalculation()
{
    float destValue = getKeypadProcessorWholeInput().toFloat();
    calculate(destValue);
}
