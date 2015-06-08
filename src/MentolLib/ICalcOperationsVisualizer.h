#ifndef ICALCOPERATIONSVISUALIZER_H
#define ICALCOPERATIONSVISUALIZER_H

#include "ICalcOperation.h"
#include <list>

class ICalcOperationsVisualizer
{
public:
    struct Context
    {
        virtual ~Context() {}
    };

    virtual ~ICalcOperationsVisualizer() {}
    virtual QString GetResult(const std::list<ICalcOperation_shptr>& allOps, ICalcOperationsVisualizer::Context* context = nullptr) const = 0;
};

ICalcOperationsVisualizer* CreateCalcOperationsTextualVisualizer();
ICalcOperationsVisualizer* CreateCalcOperationsSvgVisualizer();

struct ISvgVisualizerContext : public ICalcOperationsVisualizer::Context
{
    virtual ~ISvgVisualizerContext() {}
    virtual int SvgCanvasWidth() const = 0;
    virtual int SvgCanvasHeight() const = 0;
};

ISvgVisualizerContext* CreateSvgVisualizerContext(int svgCanvasWidth, int svgCanvasHeight);

#endif // ICALCOPERATIONSVISUALIZER_H
