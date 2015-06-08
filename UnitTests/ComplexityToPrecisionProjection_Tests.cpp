#include "AutoTest.h"

#include "src/MentolLib/ICalcResolver.h"
#include "src/MentolLib/ComplexityToPrecisionProjection.h"

class ComplexityToPrecisionProjection_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void GetDataPoints_ReturnsSingleDataPointOf100Precision_WhenNeededOperationsEmpty();
    void GetDataPoints_ReturnsSingleDataPointOf100Precision_WhenFullPrecisionComesWithSingleOperationAlready();
    void GetDataPoints_ReturnsTwoDataPoints_WhenTwoDifferentPrecisionsPossible();
    void GetDataPoints_ReturnsMultipleDataPoints_WhenMultipleDifferentPrecisionsPossible();
    void GetDataPoints_ReturnsMultipleDataPoints_ForSpecialLimitValue();
    void GetDataPoints_ReturnsComplexityAsWell_Always();
};

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsSingleDataPointOf100Precision_WhenNeededOperationsEmpty()
{
    const double destValue = 1.; // no operations needed to achieve this value
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 1U);
    QCOMPARE(dataPoints.front().second, 100); // 100%
}

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsSingleDataPointOf100Precision_WhenFullPrecisionComesWithSingleOperationAlready()
{
    const double destValue = 2.; // single operations needed to achieve this value with _full_ precision
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 1U);
    QCOMPARE(dataPoints.front().second, 100); // 100%
}

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsTwoDataPoints_WhenTwoDifferentPrecisionsPossible()
{
    const double destValue = 1.1; // only two operations needed to achieve this value with _full_ precision
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 2U);
    QCOMPARE(dataPoints.front().second, 91);              // 1.1 ≈ 1.0         with  91% precision
    QCOMPARE(std::next(dataPoints.begin())->second, 100); // 1.1 ≈ 1.0 + 0.1   with 100% precision
}

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsMultipleDataPoints_WhenMultipleDifferentPrecisionsPossible()
{
    const double destValue = 307;
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 4U);
    QCOMPARE(std::next(dataPoints.begin(), 0)->second,  65);
    QCOMPARE(std::next(dataPoints.begin(), 1)->second,  70);
    QCOMPARE(std::next(dataPoints.begin(), 2)->second,  98);
    QCOMPARE(std::next(dataPoints.begin(), 3)->second, 100);
}

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsMultipleDataPoints_ForSpecialLimitValue()
{
    const double destValue = 1.4;
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 3U);
    QCOMPARE(std::next(dataPoints.begin(), 0)->second,  71);
    QCOMPARE(std::next(dataPoints.begin(), 1)->second,  93);
    QCOMPARE(std::next(dataPoints.begin(), 2)->second, 100);
}

void ComplexityToPrecisionProjection_Tests::GetDataPoints_ReturnsComplexityAsWell_Always()
{
    const double destValue = 1.4;
    ICalcResolver_shptr resolver = CreateCalcResolver();
    resolver->SetDestValue(destValue);
    ComplexityToPrecisionProjection complexityProjection;

    auto dataPoints = complexityProjection.GetDataPoints(destValue);

    QCOMPARE(dataPoints.size(), 3U);
    QCOMPARE(std::next(dataPoints.begin(), 0)->first, 0);
    QCOMPARE(std::next(dataPoints.begin(), 1)->first, 2);
    QCOMPARE(std::next(dataPoints.begin(), 2)->first, 4);
}

#include "ComplexityToPrecisionProjection_Tests.moc"

DECLARE_TEST(ComplexityToPrecisionProjection_Tests)

