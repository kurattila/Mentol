#include "CalcResolver.h"
#include "calcoperation.h"
//#include <QDebug>

ICalcResolver_shptr CreateCalcResolver()
{
    return std::make_shared<CalcResolver>();
}

CalcResolver::CalcResolver()
    : m_DestValue(0.)
    , m_AcceptedTolerancePercents(15)
{
    m_pAllOperationsCollection.reset( CreateKnownOperationsCollection() );
}

void CalcResolver::SetDestValue(double destValue)
{
    m_DestValue = destValue;
}

void CalcResolver::SetTolerancePercents(int acceptedTolerancePercents)
{
    m_AcceptedTolerancePercents = acceptedTolerancePercents;
}

bool isGreaterThanZero(double value)
{
    return value > -1e-5;
}

static double getDistanceUsingSumOfOperations(double destValue, const std::list<ICalcOperation_shptr>& operationsToSumUp)
{
    if (operationsToSumUp.empty())
        return destValue - 1.; // "no operation" means multiplying by 1.0

    double distanceToDestinationValue = destValue;
    for (auto it = operationsToSumUp.begin(); it != operationsToSumUp.end(); ++it)
    {
        const ICalcOperation_shptr& op = *it;
        distanceToDestinationValue -= op->GetFinalMultiplier();
    }

    return distanceToDestinationValue;
}

ICalcOperation_shptr CalcResolver::getOperationWithBestConvergence(double destinationValue
                                                                 , const std::list<ICalcOperation_shptr>& allKnownOperations
                                                                 , const std::list<ICalcOperation_shptr>& convergenceOpsSoFar
                                                                 , CalcResolver::MinDistanceLookupStrategy lookupStrategy) const
{
//    qDebug() << "--- getOperationWithBestConvergence() ---";

    std::list<ICalcOperation_shptr> opsForProbing(convergenceOpsSoFar);
    auto distanceBefore = getDistanceUsingSumOfOperations(destinationValue, opsForProbing);
    if (opsForProbing.empty())
        distanceBefore = std::numeric_limits<double>::max();

    ICalcOperation_shptr opWithBestConvergence;
    double bestConvergenceDistance = distanceBefore;
    for (auto it = allKnownOperations.begin(); it != allKnownOperations.end(); ++it)
    {
        const ICalcOperation_shptr& op = *it;
        opsForProbing.push_back(op);
        auto distanceAfter = getDistanceUsingSumOfOperations(destinationValue, opsForProbing);
//        qDebug() << op->GetFinalMultiplier() << "(" << op->GetMultiplier() << ") has 'distanceAfter' of " << distanceAfter;
        if (fabs(distanceAfter) < bestConvergenceDistance )
        {
            if (lookupStrategy == CalcResolver::RawAbsoluteDistance || isGreaterThanZero(distanceAfter))
            {
                bestConvergenceDistance = fabs(distanceAfter);
                opWithBestConvergence = op;
            }
        }
        opsForProbing.pop_back();
    }

//    if (convergenceOpsSoFar.empty() && fabs(1.0 - opWithBestConvergence->GetFinalMultiplier()) < 1e-5)
//        opWithBestConvergence.reset();

    return opWithBestConvergence;
}

static bool operationsEnoughForTolerance(double destValue, int acceptedTolerancePercents, const std::list<ICalcOperation_shptr>& operations)
{
//    double acceptedTolerance = destValue * acceptedTolerancePercents / 100.;
    double currentDistance = getDistanceUsingSumOfOperations(destValue, operations);
    double currentErrorPercents = fabs(currentDistance) / (destValue / 100.);
//    if (fabs(currentDistance) <= acceptedTolerance)
//    double niceAcceptedTolerancePercents = std::min(100., acceptedTolerancePercents + .4999);
//    if (round(currentErrorPercents) <= niceAcceptedTolerancePercents)
    if (round(currentErrorPercents) <= acceptedTolerancePercents)
        return true;
    else
        return false;
}

void CalcResolver::updateBestMatchOpsWhenResultImproves(std::list<ICalcOperation_shptr>& bestMatchOps, int& minAccumulatedComplexityFound, const std::list<ICalcOperation_shptr>& operationsToSumUp) const
{
    int accumulatedComplexity = this->GetAccumulatedComplexity(operationsToSumUp);
    int complexityImprovement = minAccumulatedComplexityFound - accumulatedComplexity;
    if (complexityImprovement < 0) // we won't tolerate an increased complexity,
        return;                    // but "no improvement in complexity" can still mean an "improvement in calculation result's error"

    double currentError = fabs(getDistanceUsingSumOfOperations(m_DestValue, operationsToSumUp));
    double minErrorSoFar = fabs(getDistanceUsingSumOfOperations(m_DestValue, bestMatchOps));
    double calcErrorImprovement = minErrorSoFar - currentError;
    if (complexityImprovement > 0 || calcErrorImprovement > 1e-5)
    {
        minAccumulatedComplexityFound = accumulatedComplexity;
        bestMatchOps = operationsToSumUp;
    }
}

void ensureAtLeastIdentityOperation(std::list<ICalcOperation_shptr>& ops)
{
    if (ops.empty())
    {
        ops.push_back( CreateCalcOperation(1.0) );
    }
}

std::list<ICalcOperation_shptr> CalcResolver::GetNeededOperations() const
{
    std::list<ICalcOperation_shptr> bestMatchOps;

    if (operationsEnoughForTolerance(m_DestValue, m_AcceptedTolerancePercents, bestMatchOps))
    {
        ensureAtLeastIdentityOperation(bestMatchOps);
        return bestMatchOps; // close to 1.0 with accepted tolerance, so no mental calculations needed to do
    }


    int minAccumulatedComplexityFound = std::numeric_limits<int>::max();

    for (int maxComplexityOfSingleOperation = 1; maxComplexityOfSingleOperation < 5; ++maxComplexityOfSingleOperation)
    {
        std::list<ICalcOperation_shptr> operationsToSumUp;

        m_pAllOperationsCollection->SetMaxComplexity(maxComplexityOfSingleOperation);
        std::list<ICalcOperation_shptr> allKnownOperations = m_pAllOperationsCollection->GetAllKnownOperations();

        const int maxIterationsCount = 10; // prevent infinite loop of calculation
        for (int iterationsCount = 0; iterationsCount < maxIterationsCount; ++iterationsCount)
        {
            ICalcOperation_shptr currentBestOperation = getOperationWithBestConvergence(m_DestValue, allKnownOperations, operationsToSumUp, CalcResolver::RawAbsoluteDistance);
            if (!currentBestOperation.get())
                break;

            operationsToSumUp.push_back(currentBestOperation);
            if (operationsEnoughForTolerance(m_DestValue, m_AcceptedTolerancePercents, operationsToSumUp))
            {
                updateBestMatchOpsWhenResultImproves(bestMatchOps, minAccumulatedComplexityFound, operationsToSumUp);
                break;
            }
            else
            {
                operationsToSumUp.pop_back();
                currentBestOperation = getOperationWithBestConvergence(m_DestValue, allKnownOperations, operationsToSumUp, CalcResolver::AllowFurtherAdditions);
                if (currentBestOperation.get())
                    operationsToSumUp.push_back(currentBestOperation);
            }

            if (operationsEnoughForTolerance(m_DestValue, m_AcceptedTolerancePercents, operationsToSumUp))
            {
                updateBestMatchOpsWhenResultImproves(bestMatchOps, minAccumulatedComplexityFound, operationsToSumUp);
                break;
            }
        }
    }

    return bestMatchOps;
}

int CalcResolver::GetAccumulatedComplexity(const std::list<ICalcOperation_shptr>& opsToSumUp) const
{
    int accumulatedComplexity = 0;
    for (auto it = opsToSumUp.begin(); it != opsToSumUp.end(); ++it)
    {
        const ICalcOperation_shptr& op = *it;
        accumulatedComplexity += op->GetComplexityDistance();

        if (it != opsToSumUp.begin())
            accumulatedComplexity++; // count also additions of those multiplies: complexity(0.1 + 0.5) = complexity(0.1) + complexity(0.5) + 1 for addition
    }
    return accumulatedComplexity;
}

int CalcResolver::GetPrecision(const std::list<ICalcOperation_shptr> &opsToSumUp) const
{
    if (opsToSumUp.empty())
        return 100;

    double realResult = 0;
    for (const ICalcOperation_shptr& op: opsToSumUp)
    {
        realResult = realResult + op->GetFinalMultiplier();
    }

    double roundedPrecision = round(100 * (realResult / m_DestValue));
    return roundedPrecision;
}
