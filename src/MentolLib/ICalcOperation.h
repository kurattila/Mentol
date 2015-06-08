#ifndef ICALCOPERATION_H
#define ICALCOPERATION_H

#include <memory>
#include <QString>

class ICalcOperation;
typedef std::shared_ptr<ICalcOperation> ICalcOperation_shptr;

class ICalcOperation
{
public:
    virtual ~ICalcOperation() {}

    virtual int GetComplexityDistance() const = 0;
    virtual double GetFinalMultiplier() const = 0;

    virtual void SetMultiplier(double multiplier) = 0;
    virtual double GetMultiplier() const = 0;
    virtual QString GetHumanReadableMultiplier() const = 0;

    virtual void SetPrevOperation(ICalcOperation_shptr prevOperation) = 0;
    virtual const ICalcOperation_shptr& GetPrevOperation() const = 0;
};

ICalcOperation_shptr CreateCalcOperation(double multiplier);
ICalcOperation_shptr CreateCalcOperation(double multiplier, ICalcOperation_shptr prevOperation);

#endif // ICALCOPERATION_H
