#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include <memory>
#include "ComplexityToPrecision_Model.h"
#include "KeypadProcessor.h"


class MainViewModel : public QObject
{
    Q_OBJECT
public:
    explicit MainViewModel(QObject *parent = 0);

    Q_INVOKABLE void calculate(float destValue);
    Q_INVOKABLE void setUserPreferenceOfComplexity(int userPreferredtolerancePercents);
    Q_INVOKABLE void keypadButtonPressed(QString buttonText);

    Q_PROPERTY(QString textualVisualization READ getTextualVisualization NOTIFY textualVisualizationChanged)
    Q_PROPERTY(ComplexityToPrecision_Model* complexityDistribution READ getComplexityDistribution NOTIFY complexityDistributionChanged)
    Q_PROPERTY(bool keypadShown READ getKeypadShown WRITE setKeypadShown NOTIFY keypadShownChanged)
    Q_PROPERTY(QString keypadProcessorWholeInput READ getKeypadProcessorWholeInput NOTIFY keypadProcessorWholeInputChanged)

signals:
    void textualVisualizationChanged();
    void complexityDistributionChanged();
    void keypadShownChanged();
    void keypadProcessorWholeInputChanged();

public slots:

protected:
    virtual void updateOperationsVisualization(int tolerancePercents);

private:
    const QString& getTextualVisualization() const;
    QString m_TextualVisualization;

    ComplexityToPrecision_Model* getComplexityDistribution();
    std::unique_ptr<ComplexityToPrecision_Model> m_ComplexityToPrecision_Model;
    std::unique_ptr<KeypadProcessor> m_KeypadProcessor;

    bool getKeypadShown() const { return m_KeypadShown; }
    void setKeypadShown(bool shown);
    bool m_KeypadShown;

    QString getKeypadProcessorWholeInput() const;

    void hideKeypad();
    void restartCalculation();
};

#endif // MAINVIEWMODEL_H
