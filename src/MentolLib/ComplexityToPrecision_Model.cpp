
#include "ComplexityToPrecision_Model.h"


ComplexityToPrecision_Model::ComplexityToPrecision_Model(QObject *parent)
    : QAbstractListModel(parent)
    , m_DestValue(1.)
    , m_ComplexityToPrecisionProjection( new ComplexityToPrecisionProjection() )
    , m_UserPreferredComplexityIndex(-1)
{
}

void ComplexityToPrecision_Model::SetDestValue(double destValue)
{
    m_DestValue = destValue;

    beginResetModel();
    m_ComplexityDistribution = m_ComplexityToPrecisionProjection->GetDataPoints(destValue);
    m_ComplexityDistribution.reverse();
    endResetModel();
}

void ComplexityToPrecision_Model::SetUserPreferredIndexOfComplexity(int complexityIndex)
{
    int oldIndex = m_UserPreferredComplexityIndex;
    m_UserPreferredComplexityIndex = complexityIndex;

    QVector<int> changedRoles = { IsUserPreferredRole };
    auto qtOldIndex = createIndex(oldIndex, 0);
    auto qtNewIndex = createIndex(m_UserPreferredComplexityIndex, 0);
    emit dataChanged(qtOldIndex, qtOldIndex, changedRoles);
    emit dataChanged(qtNewIndex, qtNewIndex, changedRoles);
}

std::list< std::pair<int, int> > ComplexityToPrecision_Model::GetCurrentComplexityDistribution() const
{
    return m_ComplexityDistribution;
}

int ComplexityToPrecision_Model::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(m_ComplexityDistribution.size());
}

float getComplexityNormalized(int complexity)
{
    static const float maxDisplayedComplexity = 8;
    return complexity / maxDisplayedComplexity;
}

QString getHumanReadableComplexityPhrase(int complexity)
{
    if (complexity <= 1)
        return "Very Easy";
    else if (complexity == 2)
        return "Easy";
    else if (complexity == 3)
        return "Moderate";
    else if (complexity == 4)
        return "Hard";
    else if (complexity >= 5)
        return "Very Hard";
    else
        return "---";
}

QVariant ComplexityToPrecision_Model::data(const QModelIndex& index, int role) const
{
    auto itFoundPrecision = std::next(m_ComplexityDistribution.begin(), index.row());

    if (role == PrecisionRole)
        return QVariant::fromValue(itFoundPrecision->second);
    else if (role == PrecisionTextRole)
        return getPrecisionTextualRepresentation(itFoundPrecision->second);
    else if (role == ComplexityRole)
        return QVariant::fromValue(itFoundPrecision->first);
    else if (role == ComplexityNormalizedRole)
    {
        float normalizedComplexity = getComplexityNormalized( itFoundPrecision->first );
        return QVariant::fromValue(normalizedComplexity);
    }
    else if (role == IsUserPreferredRole)
        return QVariant::fromValue( index.row() == m_UserPreferredComplexityIndex );
    else
        return "---";
}

QHash<int, QByteArray> ComplexityToPrecision_Model::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PrecisionRole] = "precision";
    roles[PrecisionTextRole] = "precisionText";
    roles[ComplexityRole] = "complexity";
    roles[ComplexityNormalizedRole] = "complexityNormalized";
    roles[IsUserPreferredRole] = "isUserPreferred";
    return roles;
}

QString ComplexityToPrecision_Model::getPrecisionTextualRepresentation(int precision) const
{
    QString textualRepresentation;

    if (precision == 100)
        textualRepresentation = "100%";
    else
    {
        if (precision < 100)
            textualRepresentation = "-";
        else
            textualRepresentation = "+";

        textualRepresentation.append(QString("%1%").arg(abs(100 - precision)));
    }

    return textualRepresentation;
}
