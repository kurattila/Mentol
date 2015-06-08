
#include "CalcOperationsVisualizer.h"
#include "ICalcOperationsToGeometryConverter.h"
#include <QString>



ICalcOperationsVisualizer* CreateCalcOperationsTextualVisualizer()
{
    return new CalcOperationsTextualVisualizer();
}

ICalcOperationsVisualizer* CreateCalcOperationsSvgVisualizer()
{
    return new CalcOperationsSvgVisualizer();
}

ISvgVisualizerContext* CreateSvgVisualizerContext(int svgCanvasWidth, int svgCanvasHeight)
{
    return new SvgVisualizerContext(svgCanvasWidth, svgCanvasHeight);
}


CalcOperationsTextualVisualizer::CalcOperationsTextualVisualizer()
{
}

QString CalcOperationsTextualVisualizer::GetResult(const std::list<ICalcOperation_shptr> &allOps, ICalcOperationsVisualizer::Context* /*context = nullptr*/) const
{
    QString displayText;
    for (auto it = allOps.begin(); it != allOps.end(); ++it)
    {
        if (it != allOps.begin())
            displayText += QString("\n+ ");

        ICalcOperation_shptr op = *it;
        while(op.get())
        {
            QString formatterString("%1");
            if (op != *it)
                formatterString = QString(" x %1");
            displayText += formatterString.arg(op->GetMultiplier());
            op = op->GetPrevOperation();
        }
    }
    return displayText;
}



SvgVisualizerContext::SvgVisualizerContext(int svgCanvasWidth, int svgCanvasHeight)
    : m_SvgCanvasWidth(svgCanvasWidth)
    , m_SvgCanvasHeight(svgCanvasHeight)
{ }

CalcOperationsSvgVisualizer::CalcOperationsSvgVisualizer()
{

}

QString CalcOperationsSvgVisualizer::GetResult(const std::list<ICalcOperation_shptr>& allOps, ICalcOperationsVisualizer::Context* context /*= nullptr*/) const
{
    QString fullSvgXml;
    ISvgVisualizerContext* svgContext = dynamic_cast<ISvgVisualizerContext*>(context);
    if (!svgContext)
        return fullSvgXml;

    int width = svgContext->SvgCanvasWidth();
    int height = svgContext->SvgCanvasHeight();

    QString header("<?xml version=\"1.0\"?>"
                   "<svg version=\"1.1\""
                   "  xmlns=\"http://www.w3.org/2000/svg\""
                   "  xmlns:xlink=\"http://www.w3.org/1999/xlink\""
                   "  width=\"%1\" height=\"%2\""
                   "  style=\"background-color: #D2B48C;\">");
    header = header.arg(width).arg(height);
    fullSvgXml += header;

    std::unique_ptr<ICalcOperationsToGeometryConverter> calcOpToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, width, height));
    auto lines = calcOpToGeometryConverter->GetTransitionLines();
    for (auto itLine = lines.begin(); itLine != lines.end(); ++itLine)
    {
        const ICalcOperationsToGeometryConverter::TransitionLine& line = *itLine;
        QString lineSvg ("    <line x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\" stroke=\"green\" stroke-width=\"1\"/>");
        lineSvg = lineSvg.arg(line.start.x())
                         .arg(line.start.y())
                         .arg(line.end.x())
                         .arg(line.end.y());
        fullSvgXml += lineSvg;
    }

    auto circles = calcOpToGeometryConverter->GetStateCircles();
    for (auto itCircle = circles.begin(); itCircle != circles.end(); ++itCircle)
    {
        const ICalcOperationsToGeometryConverter::StateCircle& circle = *itCircle;
        QString circleSvg ("    <circle cx=\"%1\" cy=\"%2\" r=\"%3\" fill=\"lightgreen\" stroke=\"green\" stroke-width=\"1\"/>");
        circleSvg = circleSvg.arg(circle.center.x())
                             .arg(circle.center.y())
                             .arg(circle.radius);
        fullSvgXml += circleSvg;

        QString textSvg("    <text x=\"%1\" y=\"%2\" font-size=\"15px\" style=\"text-anchor: middle;\">%3</text>");
        textSvg = textSvg.arg(circle.center.x())
                         .arg(circle.center.y() + circle.radius / 5)
                         .arg(circle.text);
        fullSvgXml += textSvg;
    }

    QString footer("</svg>");
    fullSvgXml += footer;

    return fullSvgXml;
}



