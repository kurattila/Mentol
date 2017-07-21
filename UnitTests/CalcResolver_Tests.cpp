#include "AutoTest.h"

#include "src/MentolLib/ICalcResolver.h"

class CalcResolver_Tests : public QObject
{
    Q_OBJECT
public:
    CalcResolver_Tests()
    {
        m_CalcResolver = CreateCalcResolver();
    }

private slots:
    void GetNeededOperations_ReturnsIdentity_ForOne();
    void GetNeededOperations_ReturnsSingleBaseOperation_ForExactMatch();
    void GetNeededOperations_ReturnsSingleBaseOperation_ForExactMatchAboveOne();
    void GetNeededOperations_ReturnsNearestSingleBaseOperation_WhenHugeToleranceAccepted();
    void GetNeededOperations_ReturnsIdentity_WhenHugeToleranceAccepted();
    void GetNeededOperations_ReturnsExactMatchingTwoOps_WhenTinyToleranceAccepted();
    void GetNeededOperations_ReturnsHalfTenthAndTenth_ForZeroPointSeven();
    void GetNeededOperations_ReturnsHalfTenthTenthTenth_ForZeroPointEight();
    void GetNeededOperations_ReturnsComplexityOne_ForHugeTolerance();
    void GetNeededOperations_ReturnsComplexityThree_ForModerateTolerance();
    void GetNeededOperations_ReturnsComplexityFour_ForSmallTolerance();
    void GetNeededOperations_Returns_ForZeroPointThirteen();
    void GetNeededOperations_Returns_TwoTwoTwoForEight();
    void GetNeededOperations_CanHandleZeroTolerance_WhenFullPrecisionPossible();
    void GetNeededOperations_WillReturnNoOperations_WhenDestValueOperationsCantBeComputed();

    void GetAccumulatedComplexity_Returns_ZeroForNoOps();
    void GetAccumulatedComplexity_ReturnsSingleOpComplexity_ForSingleOp();
    void GetAccumulatedComplexity_ReturnsSumOfComplexitiesPlusAdditions_ForMultipleOps();

private:
    ICalcResolver_shptr m_CalcResolver;
};

void CalcResolver_Tests::GetNeededOperations_ReturnsIdentity_ForOne()
{
    m_CalcResolver->SetDestValue(1.);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(neededOperations.front()->GetFinalMultiplier(), 1.);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsSingleBaseOperation_ForExactMatch()
{
    m_CalcResolver->SetDestValue(0.5);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    ICalcOperation_shptr firstOp = neededOperations.front();
    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(firstOp->GetFinalMultiplier(), 0.5);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsSingleBaseOperation_ForExactMatchAboveOne()
{
    m_CalcResolver->SetDestValue(2.0);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    ICalcOperation_shptr firstOp = neededOperations.front();
    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(firstOp->GetFinalMultiplier(), 2.);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsNearestSingleBaseOperation_WhenHugeToleranceAccepted()
{
    m_CalcResolver->SetDestValue(0.7);
    m_CalcResolver->SetTolerancePercents(40);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    ICalcOperation_shptr firstOp = neededOperations.front();
    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(firstOp->GetFinalMultiplier(), 0.5);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsIdentity_WhenHugeToleranceAccepted()
{
    m_CalcResolver->SetDestValue(0.7);
    m_CalcResolver->SetTolerancePercents(50);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(neededOperations.front()->GetFinalMultiplier(), 1.);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsExactMatchingTwoOps_WhenTinyToleranceAccepted()
{
    m_CalcResolver->SetDestValue(0.6);
    m_CalcResolver->SetTolerancePercents(1);

    auto neededOperations = m_CalcResolver->GetNeededOperations();

    ICalcOperation_shptr firstOp = neededOperations.front();
    ICalcOperation_shptr secondOp = neededOperations.back();
    QCOMPARE(neededOperations.size(), 2UL);
    QCOMPARE(firstOp->GetFinalMultiplier(), 0.5);
    QCOMPARE(secondOp->GetFinalMultiplier(), 0.1);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsHalfTenthAndTenth_ForZeroPointSeven()
{
    m_CalcResolver->SetDestValue(0.7);
    m_CalcResolver->SetTolerancePercents(1);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 0.7 ≈ 0.5 + 2x0.1 (better than 0.5 + 0.1 + 0.1, because of lower complexity)

    QCOMPARE(neededOperations.size(), 2UL);
    auto op = neededOperations.begin();
    QCOMPARE((*op)->GetFinalMultiplier(), 0.5);
    ++op;
    QCOMPARE((*op)->GetFinalMultiplier(), 0.2);
    QCOMPARE((*op)->GetMultiplier(), 0.1);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsHalfTenthTenthTenth_ForZeroPointEight()
{
    m_CalcResolver->SetDestValue(0.8);
    m_CalcResolver->SetTolerancePercents(1);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 0.8 ≈ 0.5 + 0.1 + 0.1 + 0.1
                                                                   // 0.8 ≈ 0.1 * 2 * 2 * 2

    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(neededOperations.front()->GetFinalMultiplier(), 0.8);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsComplexityOne_ForHugeTolerance()
{
    m_CalcResolver->SetDestValue(7.4);
    m_CalcResolver->SetTolerancePercents(50);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 7.4 ≈ 10.0

    QCOMPARE(neededOperations.size(), 1UL);
    QCOMPARE(neededOperations.front()->GetComplexityDistance(), 1);
    QCOMPARE(neededOperations.front()->GetFinalMultiplier(), 10.);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsComplexityThree_ForModerateTolerance()
{
    m_CalcResolver->SetDestValue(7.4);
    m_CalcResolver->SetTolerancePercents(25);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 7.4 ≈ 2 x 2 x 2
                                                                   // (   lower complexity than 7.4 ≈ 0.5 x 10 + 2.0
                                                                   // and lower complexity than 7.4 ≈ 2.0 + 2.0 + 2.0)

    QCOMPARE(neededOperations.size(), 1UL);
    auto op = neededOperations.front();
    QCOMPARE(op->GetFinalMultiplier(), 8.); // 2 x 2 x 2
    auto op2 = op->GetPrevOperation();
    QCOMPARE(op2->GetMultiplier(), 2.);
    auto op3 = op->GetPrevOperation();
    QCOMPARE(op3->GetMultiplier(), 2.);
}

void CalcResolver_Tests::GetNeededOperations_ReturnsComplexityFour_ForSmallTolerance()
{
    m_CalcResolver->SetDestValue(7.42);
    m_CalcResolver->SetTolerancePercents(5);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 7.42 ≈ 0.5x10 + 2 + 0.5
                                                                   // (lower complexity than 7.42 ≈ 2 + 2 + 2 + 1 + 0.5)
    QCOMPARE(neededOperations.size(), 3UL);
    auto op = neededOperations.begin();
    QCOMPARE((*op)->GetFinalMultiplier(), 5.); // 5 == 0.5 x 10.0
    QCOMPARE((*op)->GetPrevOperation()->GetMultiplier(), 10.);
    ++op;
    QCOMPARE((*op)->GetFinalMultiplier(), 2.);
    ++op;
    QCOMPARE((*op)->GetFinalMultiplier(), 0.5);
}

void CalcResolver_Tests::GetNeededOperations_Returns_ForZeroPointThirteen()
{
    m_CalcResolver->SetDestValue(0.13);
    m_CalcResolver->SetTolerancePercents(20);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 0.13 ≈ 0.5 x 0.5 x 0.5
                                                                   // (better than 0.1 + 0.05)

    QCOMPARE(neededOperations.size(), 1UL);
    auto op = neededOperations.begin();
    QCOMPARE((*op)->GetFinalMultiplier(), 0.125);
}

void CalcResolver_Tests::GetNeededOperations_Returns_TwoTwoTwoForEight()
{
    m_CalcResolver->SetDestValue(8.0);
    m_CalcResolver->SetTolerancePercents(20);

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 8.0 ≈ 2 x 2 x 2
                                                                   // (better than 2 + 2 + 2 + 2)

    QCOMPARE(neededOperations.size(), 1UL);
    auto op = neededOperations.front();
    QCOMPARE(op->GetFinalMultiplier(), 8.); // 2 x 2 x 2
    auto op2 = op->GetPrevOperation();
    QCOMPARE(op2->GetMultiplier(), 2.);
    auto op3 = op->GetPrevOperation();
    QCOMPARE(op3->GetMultiplier(), 2.);
}

void CalcResolver_Tests::GetNeededOperations_CanHandleZeroTolerance_WhenFullPrecisionPossible()
{
    m_CalcResolver->SetDestValue(1.1);
    m_CalcResolver->SetTolerancePercents(0); // require _full_, 100% precision

    auto neededOperations = m_CalcResolver->GetNeededOperations(); // 1.1 ≈ 1 + 0.1

    QCOMPARE(neededOperations.size(), 2UL);
    auto op = neededOperations.begin();
    QCOMPARE((*op)->GetFinalMultiplier(), 1.);
    op++;
    QCOMPARE((*op)->GetFinalMultiplier(), .1);
}

void CalcResolver_Tests::GetNeededOperations_WillReturnNoOperations_WhenDestValueOperationsCantBeComputed()
{
    m_CalcResolver->SetDestValue(0.00001);
    m_CalcResolver->SetTolerancePercents(15);

    std::list<ICalcOperation_shptr> neededOperations = m_CalcResolver->GetNeededOperations();

    QVERIFY(neededOperations.empty());
}

void CalcResolver_Tests::GetAccumulatedComplexity_Returns_ZeroForNoOps()
{
    std::list<ICalcOperation_shptr> opsToSumUp;

    int accumulatedComplexity = m_CalcResolver->GetAccumulatedComplexity(opsToSumUp);

    QCOMPARE(accumulatedComplexity, 0);
}

void CalcResolver_Tests::GetAccumulatedComplexity_ReturnsSingleOpComplexity_ForSingleOp()
{
    std::list<ICalcOperation_shptr> opsToSumUp;
    auto op1 = CreateCalcOperation(0.5);
    auto op2 = CreateCalcOperation(0.1, op1);
    opsToSumUp.push_back(op2);

    int accumulatedComplexity = m_CalcResolver->GetAccumulatedComplexity(opsToSumUp);

    QCOMPARE(accumulatedComplexity, 2);
}

void CalcResolver_Tests::GetAccumulatedComplexity_ReturnsSumOfComplexitiesPlusAdditions_ForMultipleOps()
{
    std::list<ICalcOperation_shptr> opsToSumUp;
    auto op1 = CreateCalcOperation(0.5);
    auto op2 = CreateCalcOperation(0.1, op1);
    auto op3 = CreateCalcOperation(10.);
    opsToSumUp.push_back(op2);
    opsToSumUp.push_back(op3);

    int accumulatedComplexity = m_CalcResolver->GetAccumulatedComplexity(opsToSumUp);

    QCOMPARE(accumulatedComplexity, 3 + 1); // 3 multiplications and 1 addition operation
}

#include "CalcResolver_Tests.moc"

DECLARE_TEST(CalcResolver_Tests)

