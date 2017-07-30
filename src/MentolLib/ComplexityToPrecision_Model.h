#ifndef COMPLEXITYTOPRECISION_MODELH
#define COMPLEXITYTOPRECISION_MODELH

#include <QAbstractListModel>
#include <memory>
#include "ComplexityToPrecisionProjection.h"

class ComplexityToPrecision_Model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ComplexityToPrecision_Model(QObject *parent = 0);

    void SetDestValue(double destValue);
    double GetDestValue() const { return m_DestValue; }
    void SetUserPreferredIndexOfComplexity(int complexityIndex);
    std::list<std::pair<int, int> > GetCurrentComplexityDistribution() const;

    enum LectureCollectionFactoryRoles
    {
        PrecisionRole = Qt::UserRole + 1,
        PrecisionTextRole,
        ComplexityRole,
        ComplexityNormalizedRole,
        IsUserPreferredRole
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

signals:

public slots:

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QString getPrecisionTextualRepresentation(int precision) const;
    double m_DestValue;
    std::unique_ptr<ComplexityToPrecisionProjection> m_ComplexityToPrecisionProjection;
    std::list< std::pair<int, int> > m_ComplexityDistribution;
    int m_UserPreferredComplexityIndex;
};

#endif // COMPLEXITYTOPRECISION_MODELH
