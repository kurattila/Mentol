#include "MainViewModel.h"

#include "src/MentolLib/ICalcResolver.h"
#include "src/MentolLib/ICalcOperationsVisualizer.h"



MainViewModel::MainViewModel(QObject *parent)
    : QObject(parent)
    , m_ComplexityToPrecision_Model(new ComplexityToPrecision_Model())
{
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

const QString &MainViewModel::getTextualVisualization() const
{
    return m_TextualVisualization;
}

ComplexityToPrecision_Model *MainViewModel::getComplexityDistribution()
{
    return m_ComplexityToPrecision_Model.get();
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
