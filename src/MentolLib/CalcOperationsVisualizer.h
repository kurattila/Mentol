#ifndef CALCOPERATIONSVISUALIZER_H
#define CALCOPERATIONSVISUALIZER_H

#include "ICalcOperationsVisualizer.h"

class CalcOperationsTextualVisualizer : public ICalcOperationsVisualizer
{
public:
    CalcOperationsTextualVisualizer();
    virtual ~CalcOperationsTextualVisualizer() { }

    virtual QString GetResult(const std::list<ICalcOperation_shptr>& allOps, ICalcOperationsVisualizer::Context* context = nullptr) const;
};

class CalcOperationsSvgVisualizer : public ICalcOperationsVisualizer
{
public:
    CalcOperationsSvgVisualizer();
    virtual ~CalcOperationsSvgVisualizer() { }

    virtual QString GetResult(const std::list<ICalcOperation_shptr>& allOps, ICalcOperationsVisualizer::Context* context = nullptr) const;
};

struct SvgVisualizerContext : public ISvgVisualizerContext
{
public:
    SvgVisualizerContext(int svgCanvasWidth, int svgCanvasHeight);
    virtual ~SvgVisualizerContext() { }

    virtual int SvgCanvasWidth() const { return m_SvgCanvasWidth; }
    virtual int SvgCanvasHeight() const { return m_SvgCanvasHeight; }

private:
    int m_SvgCanvasWidth;
    int m_SvgCanvasHeight;
};

#endif // CALCOPERATIONSVISUALIZER_H
