#include "calcoperation.h"
#include <QString>

//-----------------------------------------------------
//    CalcOperation factory methods
//-----------------------------------------------------
ICalcOperation_shptr CreateCalcOperation(double multiplier)
{
    ICalcOperation_shptr calcOperation = std::make_shared<CalcOperation>();
    calcOperation->SetMultiplier(multiplier);
    return calcOperation;
}

ICalcOperation_shptr CreateCalcOperation(double multiplier, ICalcOperation_shptr prevOperation)
{
    ICalcOperation_shptr calcOperation = CreateCalcOperation(multiplier);
    calcOperation->SetPrevOperation(prevOperation);
    return calcOperation;
}

//-----------------------------------------------------
//    CalcOperation
//-----------------------------------------------------
CalcOperation::CalcOperation()
    : m_pPrevOperation(nullptr)
    , m_Multiplier(0.)
{
}

CalcOperation::~CalcOperation()
{

}

static bool isIdentityOperation(const ICalcOperation* op)
{
    return 1e-5 > fabs(op->GetFinalMultiplier() - 1.);
}

int CalcOperation::GetComplexityDistance() const
{
    int complexityDistance = 0;
    const ICalcOperation* pCurrentOperation = this;
    while (pCurrentOperation)
    {
        if (isIdentityOperation(pCurrentOperation))
            break; // complexity of zero means "nothing to calculate"
        complexityDistance++;

        const CalcOperation* calcOperationImpl = dynamic_cast<const CalcOperation*>(pCurrentOperation);
        if (calcOperationImpl)
            pCurrentOperation = calcOperationImpl->m_pPrevOperation.get();
        else
            pCurrentOperation = nullptr;
    }

    return complexityDistance;
}

double CalcOperation::GetFinalMultiplier() const
{
    double finalMultiplier = this->GetMultiplier();
    if (m_pPrevOperation.get())
        finalMultiplier = finalMultiplier * m_pPrevOperation->GetFinalMultiplier();

    return finalMultiplier;
}

void CalcOperation::SetMultiplier(double multiplier)
{
    m_Multiplier = multiplier;
}

double CalcOperation::GetMultiplier() const
{
    return m_Multiplier;
}

QString CalcOperation::GetHumanReadableMultiplier() const
{
    if (m_Multiplier >= 1)
        return QString("&#215;%1").arg(m_Multiplier);
    else
        return QString("&#247;%1").arg(1/m_Multiplier);
}

void CalcOperation::SetPrevOperation(ICalcOperation_shptr prevOperation)
{
    m_pPrevOperation = prevOperation;
}

const ICalcOperation_shptr& CalcOperation::GetPrevOperation() const
{
    return m_pPrevOperation;
}
