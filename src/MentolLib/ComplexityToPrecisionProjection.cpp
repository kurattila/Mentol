
#include "ComplexityToPrecisionProjection.h"
#include "src/MentolLib/ICalcResolver.h"
#include <map>
#include <algorithm>


ComplexityToPrecisionProjection::ComplexityToPrecisionProjection()
{
}

static void updateComplexityToPrecisionMap(std::map<int, int>& complexityToPrecisionMap, int complexity, int precision)
{
    double precisionToUse = precision;
    auto itFound = complexityToPrecisionMap.find(complexity);
    if (itFound != complexityToPrecisionMap.end())
    {
        int currentPrecision = itFound->second;
        if (currentPrecision > precisionToUse)
            precisionToUse = currentPrecision;
    }

    complexityToPrecisionMap[complexity] = precisionToUse;
}

std::list< std::pair<int, int> > ComplexityToPrecisionProjection::GetDataPoints(double destValue) const
{
    std::map<int, int> complexityToPrecisionMap;

    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);

    for (int tolerancePercents = 0; tolerancePercents < 40; tolerancePercents++)
    {
        resolver->SetTolerancePercents(tolerancePercents);

        auto neededOperations = resolver->GetNeededOperations();
        int accumulatedComplexity = resolver->GetAccumulatedComplexity(neededOperations);
        updateComplexityToPrecisionMap(complexityToPrecisionMap, accumulatedComplexity, 100 - tolerancePercents);
    }

    std::list< std::pair<int, int> > dataPoints;
    for (auto complexityToPrecisionPair: complexityToPrecisionMap)
    {
        dataPoints.push_back( complexityToPrecisionPair );
    }

    return dataPoints;
}
