#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include <memory>
#include "ComplexityToPrecision_Model.h"


class MainViewModel : public QObject
{
    Q_OBJECT
public:
    explicit MainViewModel(QObject *parent = 0);

    Q_INVOKABLE void calculate(float destValue);
    Q_INVOKABLE void setUserPreferenceOfComplexity(int userPreferredtolerancePercents);

    Q_PROPERTY(QString textualVisualization READ getTextualVisualization NOTIFY textualVisualizationChanged)
    Q_PROPERTY(ComplexityToPrecision_Model* complexityDistribution READ getComplexityDistribution NOTIFY complexityDistributionChanged)

signals:
    void textualVisualizationChanged();
    void complexityDistributionChanged();

public slots:

private:
    const QString& getTextualVisualization() const;
    QString m_TextualVisualization;

    ComplexityToPrecision_Model* getComplexityDistribution();
    std::unique_ptr<ComplexityToPrecision_Model> m_ComplexityToPrecision_Model;

    void updateOperationsVisualization(int tolerancePercents);
};

#endif // MAINVIEWMODEL_H
