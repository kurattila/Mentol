#include "AutoTest.h"

#include "src/MentolLib/ICalcOperationsToGeometryConverter.h"

class CalcOperationsToGeometryConverter_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void NoOperation_DrawsOnlySingleTransitionLineWith2Circles();
    void NoOperation_DrawsOnlyStartAndEnd();
    void SingleOperation_ComposedOfTwoOperations_WillHaveThreeTransitionLines();
    void SingleOperation_NotComposedOfMany_HasCircleAtHalfTheWay();
    void TwoSimpleOperations_Have4LinesAnd4Circles();
    void TwoSimpleOperations_FirstWillBeLeftmost();
    void TwoSimpleOperations_SecondWillBeRightmost();
    void TwoSimpleOperations_OneBeingIdentity_SecondWillBeRightmost();
    void TwoSimpleOperations_LeftOperation_LineBeginsAtStartCircle();

private:
    static const int Margin = 10;
    static const int CircleRadius = 20;

    bool fuzzyCompare(double actual, double expected, double maxdiff)
    { return (qAbs(actual - expected) <= maxdiff); }
};

void CalcOperationsToGeometryConverter_Tests::NoOperation_DrawsOnlySingleTransitionLineWith2Circles()
{
    std::list<ICalcOperation_shptr> allOps;
    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 50,50) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();

    QCOMPARE(allLines.size(), 1U);
    QCOMPARE(allCircles.size(), 2U);
}

void CalcOperationsToGeometryConverter_Tests::NoOperation_DrawsOnlyStartAndEnd()
{
    int width = 100;
    int height = 100;
    std::list<ICalcOperation_shptr> allOps;
    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, width, height) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();

    const ICalcOperationsToGeometryConverter::TransitionLine& line = allLines.front();
    const ICalcOperationsToGeometryConverter::StateCircle& beginCircle = allCircles.front();
    const ICalcOperationsToGeometryConverter::StateCircle& endCircle = allCircles.back();
    QCOMPARE(line.start, QPoint(width/2,Margin + CircleRadius));
    QCOMPARE(line.end, QPoint(width/2,height - Margin - CircleRadius));
    QCOMPARE(beginCircle.center, QPoint(width/2, Margin + CircleRadius));
    QVERIFY(fuzzyCompare(beginCircle.radius, CircleRadius, 1e-5));
    QCOMPARE(endCircle.center, QPoint(width/2, height - Margin - CircleRadius));
    QVERIFY(fuzzyCompare(endCircle.radius, CircleRadius, 1e-5));
}

void CalcOperationsToGeometryConverter_Tests::SingleOperation_ComposedOfTwoOperations_WillHaveThreeTransitionLines()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(0.1);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.1, op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 100,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();
    QCOMPARE(allLines.size(), 3U);
    QCOMPARE(allCircles.size(), 4U);
}

void CalcOperationsToGeometryConverter_Tests::SingleOperation_NotComposedOfMany_HasCircleAtHalfTheWay()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op = CreateCalcOperation(0.1);
    allOps.push_back(op);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 100,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();
    QCOMPARE(allLines.front().end, QPoint(50,50));
    auto circlesIt = allCircles.begin();
    QCOMPARE((++circlesIt)->center, QPoint(50,50));
}

void CalcOperationsToGeometryConverter_Tests::TwoSimpleOperations_Have4LinesAnd4Circles()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(0.1);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.5);
    allOps.push_back(op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 100,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();
    QCOMPARE(allLines.size(), 4U);
    QCOMPARE(allCircles.size(), 4U);
}

void CalcOperationsToGeometryConverter_Tests::TwoSimpleOperations_FirstWillBeLeftmost()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(0.1);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.5);
    allOps.push_back(op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 100,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();
    auto circlesIt = allCircles.begin();
    QCOMPARE((++circlesIt)->center, QPoint(Margin + CircleRadius,50)); // 1st operation's circle
    auto linesIt = allLines.begin();
    QCOMPARE(linesIt->end, QPoint(Margin + CircleRadius, 50)); // end of line between Start and 1st Op
    linesIt++;
    QCOMPARE(linesIt->start, QPoint(Margin + CircleRadius, 50)); // start of line between 1st Op and End
}

void CalcOperationsToGeometryConverter_Tests::TwoSimpleOperations_SecondWillBeRightmost()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(0.1);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.5);
    allOps.push_back(op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 200,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    auto allCircles = opsToGeometryConverter->GetStateCircles();
    auto circlesIt = allCircles.begin();
    std::advance(circlesIt, 2);
    QCOMPARE(circlesIt->center, QPoint(200 - Margin - CircleRadius,50)); // 2nd operation's circle
    auto linesIt = allLines.begin();
    std::advance(linesIt, 2);
    QCOMPARE(linesIt->end, QPoint(200 - Margin - CircleRadius, 50)); // end of line between Start and 2nd Op
    linesIt++;
    QCOMPARE(linesIt->start, QPoint(200 - Margin - CircleRadius, 50)); // start of line between 2nd Op and End
}

void CalcOperationsToGeometryConverter_Tests::TwoSimpleOperations_OneBeingIdentity_SecondWillBeRightmost()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(2);
    ICalcOperation_shptr op2 = CreateCalcOperation(1);
    allOps.push_back(op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 200,100) );

    auto allCircles = opsToGeometryConverter->GetStateCircles();
    auto circlesIt = allCircles.begin();
    std::advance(circlesIt, 2);
    QCOMPARE(circlesIt->center, QPoint(200 - Margin - CircleRadius,50)); // 2nd operation's circle
}

void CalcOperationsToGeometryConverter_Tests::TwoSimpleOperations_LeftOperation_LineBeginsAtStartCircle()
{
    std::list<ICalcOperation_shptr> allOps;
    ICalcOperation_shptr op1 = CreateCalcOperation(2);
    ICalcOperation_shptr op2 = CreateCalcOperation(1);
    allOps.push_back(op1);
    allOps.push_back(op2);

    std::unique_ptr<ICalcOperationsToGeometryConverter> opsToGeometryConverter(
                CreateCalcOperationsToGeometryConverter(allOps, 200,100) );

    auto allLines = opsToGeometryConverter->GetTransitionLines();
    QCOMPARE(allLines.front().start, QPoint(100, Margin + CircleRadius));
}


#include "CalcOperationsToGeometryConverter_Tests.moc"

DECLARE_TEST(CalcOperationsToGeometryConverter_Tests)

