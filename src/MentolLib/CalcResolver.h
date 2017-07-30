#ifndef CALCRESOLVER_H
#define CALCRESOLVER_H

#include <memory>
#include <list>
#include "ICalcResolver.h"
#include "ICalcOperation.h"
#include "IKnownOperations.h"

class CalcResolver : public ICalcResolver
{
public:
    CalcResolver();

    void SetDestValue(double destValue);
    void SetTolerancePercents(int acceptedTolerancePercents);
    std::list<ICalcOperation_shptr> GetNeededOperations() const;
    int GetAccumulatedComplexity(const std::list<ICalcOperation_shptr>& opsToSumUp) const;
    int GetPrecision(const std::list<ICalcOperation_shptr>& opsToSumUp) const override;

private:
    enum MinDistanceLookupStrategy
    {
        AllowFurtherAdditions = 0,
        RawAbsoluteDistance
    };

    double m_DestValue;
    int    m_AcceptedTolerancePercents;
    std::unique_ptr<IKnownOperations> m_pAllOperationsCollection;

    ICalcOperation_shptr getOperationWithMinDistance(double destinationValue
                                                   , const std::list<ICalcOperation_shptr>& allKnownOperations
                                                   , MinDistanceLookupStrategy lookupStrategy) const;

    ICalcOperation_shptr getOperationWithBestConvergence(double destinationValue
                                                       , const std::list<ICalcOperation_shptr>& allKnownOperations
                                                       , const std::list<ICalcOperation_shptr>& convergenceOpsSoFar
                                                       , MinDistanceLookupStrategy lookupStrategy) const;

    void updateBestMatchOpsWhenResultImproves(std::list<ICalcOperation_shptr>& bestMatchOps
                                              , int& minAccumulatedComplexityFound
                                              , const std::list<ICalcOperation_shptr>& operationsToSumUp) const;
};

#endif // CALCRESOLVER_H
