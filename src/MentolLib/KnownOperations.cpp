#include "KnownOperations.h"

IKnownOperations* CreateKnownOperationsEmptyCollection()
{
    return new KnownOperations();
}

IKnownOperations* CreateKnownOperationsCollection()
{
    IKnownOperations* knownOperations = new KnownOperations();
    knownOperations->AddBaseOperation( CreateCalcOperation(1.) );  // identity (same as "no operation" over the source operand)
    knownOperations->AddBaseOperation( CreateCalcOperation(2.) );  // multiply by 2
    knownOperations->AddBaseOperation( CreateCalcOperation(10.) ); // multiply by 10
    knownOperations->AddBaseOperation( CreateCalcOperation(0.5) ); // divide by 2
    knownOperations->AddBaseOperation( CreateCalcOperation(0.1) ); // divide by 10
    return knownOperations;
}

KnownOperations::KnownOperations()
    : mMaxComplexity(1) // no derived operations allowed at all, by default
{ }

KnownOperations::~KnownOperations()
{
}

void KnownOperations::AddBaseOperation(ICalcOperation_shptr operation)
{
    mBaseOperations.push_back(operation);
}

void KnownOperations::SetMaxComplexity(int newMaxComplexity)
{
    mMaxComplexity = newMaxComplexity;
}

std::list<ICalcOperation_shptr> KnownOperations::GetAllKnownOperations() const
{
    refillDerivedOperationsCache();

    return mDerivedOperationsCache;
}

void addIfUnique(std::list<ICalcOperation_shptr>& list, ICalcOperation_shptr op)
{
    auto itFoundDuplicate = std::find_if(list.begin(), list.end(),
    [op](ICalcOperation_shptr other)
    {
            return fabs(other->GetFinalMultiplier() - op->GetFinalMultiplier()) < 1e-5;
    });

    // Add only if it will be a unique operation
    if (itFoundDuplicate == list.end())
        list.push_back(op);
}

void KnownOperations::refillDerivedOperationsCache() const
{
    mDerivedOperationsCache.clear();

    std::list<ICalcOperation_shptr> allOperations;
    for (auto itOp = mBaseOperations.begin(); itOp != mBaseOperations.end(); ++itOp)
    {
        const ICalcOperation_shptr& baseOp = *itOp;
        if (baseOp->GetComplexityDistance() <= mMaxComplexity)
            allOperations.push_back(baseOp);
    }
    mDerivedOperationsCache.insert(mDerivedOperationsCache.end(), allOperations.begin(), allOperations.end());

    for (int complexity=2; complexity<=mMaxComplexity; complexity++)
    {
        std::list<ICalcOperation_shptr> toBeAdded;
        for (auto itMultiplier1 = allOperations.cbegin(); itMultiplier1!=allOperations.cend(); ++itMultiplier1)
        {
            for (auto itMultiplier2 = itMultiplier1; itMultiplier2!=allOperations.end(); ++itMultiplier2)
            {
                ICalcOperation_shptr multiplier1 = *itMultiplier1;
                ICalcOperation_shptr multiplier2 = *itMultiplier2;
                ICalcOperation_shptr derivedOp = CreateCalcOperation(multiplier2->GetMultiplier(), multiplier1);

                if (derivedOp->GetComplexityDistance() == complexity)
                {
                    addIfUnique(toBeAdded, derivedOp);
                }
           }
        }
        allOperations.insert(allOperations.end(), toBeAdded.begin(), toBeAdded.end());
        mDerivedOperationsCache.insert(mDerivedOperationsCache.end(), toBeAdded.begin(), toBeAdded.end());
    }

//    if (mMaxComplexity >= 2)
//    {
//        for (auto itBaseMultiplier1 = mBaseOperations.cbegin(); itBaseMultiplier1!=mBaseOperations.cend(); ++itBaseMultiplier1)
//        {
//            auto itBaseMultiplier2 = itBaseMultiplier1;
//            for (++itBaseMultiplier2; itBaseMultiplier2!=mBaseOperations.end(); ++itBaseMultiplier2)
//            {
//                ICalcOperation_shptr baseMultiplier1 = *itBaseMultiplier1;
//                ICalcOperation_shptr baseMultiplier2 = *itBaseMultiplier2;

//                ICalcOperation_shptr derivedOp = CreateCalcOperation(baseMultiplier2->GetMultiplier(), baseMultiplier1);
//                mDerivedOperationsCache.push_back(derivedOp);
//            }
//        }
//    }

//    if (mMaxComplexity >= 3)
//    {

//    }
}
