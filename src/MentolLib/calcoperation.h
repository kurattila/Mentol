#ifndef CALCOPERATION_H
#define CALCOPERATION_H

#include "ICalcOperation.h"

class CalcOperation : public ICalcOperation
{
public:
    CalcOperation();

    virtual ~CalcOperation();

    virtual int GetComplexityDistance() const;
    virtual double GetFinalMultiplier() const;

    virtual void SetMultiplier(double multiplier);
    virtual double GetMultiplier() const;
    virtual QString GetHumanReadableMultiplier() const override;

    virtual void SetPrevOperation(ICalcOperation_shptr prevOperation);
    virtual const ICalcOperation_shptr& GetPrevOperation() const;

private:
    ICalcOperation_shptr m_pPrevOperation;
    double m_Multiplier;
};

#endif // CALCOPERATION_H
