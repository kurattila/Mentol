
#include "ICalcOperationsToGeometryConverter.h"

class CalcOperationsToGeometryConverter : public ICalcOperationsToGeometryConverter
{
public:
    CalcOperationsToGeometryConverter(const std::list<ICalcOperation_shptr>& allOps, int canvasWidth, int canvasHeight);
    virtual ~CalcOperationsToGeometryConverter();

    virtual std::list<ICalcOperationsToGeometryConverter::TransitionLine> GetTransitionLines() const;
    virtual std::list<ICalcOperationsToGeometryConverter::StateCircle> GetStateCircles() const;

private:
    std::list<ICalcOperation_shptr> m_AllOperations;
    int m_CanvasWidth;
    int m_CanvasHeight;
    static const int CanvasMargin = 10;
    static const int CircleRadius = 20;

    std::list<TransitionLine> m_Lines;
    std::list<StateCircle>    m_Circles;
};

ICalcOperationsToGeometryConverter* CreateCalcOperationsToGeometryConverter(
          const std::list<ICalcOperation_shptr>& allOps
        , int canvasWidth
        , int canvasHeight)
{
    return new CalcOperationsToGeometryConverter(allOps, canvasWidth, canvasHeight);
}



static ICalcOperationsToGeometryConverter::TransitionLine createLine(const QPoint& start, const QPoint& end)
{
    ICalcOperationsToGeometryConverter::TransitionLine line;
    line.start = start;
    line.end = end;

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();
    line.rotationAngleOfArrowHead = atan2(dy, dx); // http://stackoverflow.com/questions/6247153/angle-from-2d-unit-vector

    return line;
}

static ICalcOperationsToGeometryConverter::StateCircle createCircle(const QPoint& center, int radius, const QString& text)
{
    ICalcOperationsToGeometryConverter::StateCircle circle;
    circle.center = center;
    circle.radius = radius;
    circle.text = text;
    return circle;
}

CalcOperationsToGeometryConverter::CalcOperationsToGeometryConverter(const std::list<ICalcOperation_shptr> &allOps, int canvasWidth, int canvasHeight)
    : m_AllOperations(allOps)
    , m_CanvasWidth(canvasWidth)
    , m_CanvasHeight(canvasHeight)
{
    QPoint start(m_CanvasWidth / 2, CanvasMargin + CircleRadius);
    QPoint end(m_CanvasWidth / 2, m_CanvasHeight - CanvasMargin - CircleRadius);

    auto startCircle = createCircle(start, CircleRadius, "Start");
    m_Circles.push_back(startCircle);

    size_t mainOperationsCount = m_AllOperations.size();
    size_t mainOperationsHorizontalStep = 0;
    if (mainOperationsCount > 1)
        mainOperationsHorizontalStep = (m_CanvasWidth - 2 * CanvasMargin - 2 * CircleRadius) / (mainOperationsCount - 1);
    int mainOperationIndex = 0;
    for (auto it = m_AllOperations.begin(); it != m_AllOperations.end(); (++it, ++mainOperationIndex))
    {
        ICalcOperation_shptr op = *it;
        int subOperationsCount = op->GetComplexityDistance();
        int subOperationsDownwardsStep = (end.y() - start.y()) / (subOperationsCount + 1);
        if (subOperationsCount == 0)
            subOperationsDownwardsStep = (end.y() - start.y()) / 2;

        QPoint lineStart = start;
        QPoint currentCirclePosition(start);
        if (mainOperationsCount > 1)
        {
            currentCirclePosition.rx() = CanvasMargin + CircleRadius + mainOperationIndex * static_cast<int>(mainOperationsHorizontalStep);
        }

        while(op.get())
        {
            currentCirclePosition.ry() += subOperationsDownwardsStep;
            QPoint lineEnd = currentCirclePosition;
            auto line = createLine(lineStart, lineEnd);
            m_Lines.push_back(line);

            auto circle = createCircle(currentCirclePosition, CircleRadius, op->GetHumanReadableMultiplier() );
            m_Circles.push_back(circle);

            lineStart = currentCirclePosition;
            op = op->GetPrevOperation();
        }

        auto line = createLine(currentCirclePosition, end);
        m_Lines.push_back(line);
    }

    if (m_Lines.empty())
    {
        auto line = createLine(start, end);
        m_Lines.push_back(line);
    }

    auto endCircle = createCircle(end, CircleRadius, "End");
    m_Circles.push_back(endCircle);
}

CalcOperationsToGeometryConverter::~CalcOperationsToGeometryConverter()
{ }

std::list<ICalcOperationsToGeometryConverter::TransitionLine> CalcOperationsToGeometryConverter::GetTransitionLines() const
{
//    m_Lines.clear();
//    TransitionLine startToEnd;
//    startToEnd.start = QPoint(m_CanvasWidth / 2, CalcOperationsToGeometryConverter::CanvasMargin);
//    startToEnd.end   = QPoint(m_CanvasWidth / 2, m_CanvasHeight - CalcOperationsToGeometryConverter::CanvasMargin);

//    m_Lines.push_back(startToEnd);
    return m_Lines;
}

std::list<ICalcOperationsToGeometryConverter::StateCircle> CalcOperationsToGeometryConverter::GetStateCircles() const
{
//    std::list<StateCircle> allCircles;
//    StateCircle start, end;
//    start.center = QPoint(m_CanvasWidth / 2, CanvasMargin + CircleRadius);
//    start.radius = CircleRadius;
//    end.center = QPoint(m_CanvasWidth / 2, m_CanvasHeight - CanvasMargin - CircleRadius);
//    end.radius = CircleRadius;

//    allCircles.push_back(start);
//    allCircles.push_back(end);
    return m_Circles;
}
