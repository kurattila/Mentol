#include "AutoTest.h"

#include "src/MentolLib/ICalcOperation.h"

class CalcOperation_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void FactoryMethodOne_WillSetMultiplier();
    void FactoryMethodTwo_WillSetMultiplier();
    void FactoryMethodTwo_WillChainOperations();
    void GetMultiplier_ReturnsPreviouslySetMultiplier();
    void GetFinalMultiplier_ForNonChainedOperation_ReturnsShallowMultiplier();
    void GetFinalMultiplier_ForChainedOperations_ReturnsCumulativeMultiplier();
    void SetPrevOperation_WillChainOperations();
    void GetComplexityDistance_ForIdentityOperation_WillReturnZero();
    void GetComplexityDistance_ForSimpleOperation_WillReturnOne();
    void GetComplexityDistance_ForChainedOperations_WillReturnNumberOfOperations();

    void GetHumanReadableMultiplier_ReturnsOne_ForIdentityOperation();
    void GetHumanReadableMultiplier_ReturnsMultiplication_ForMultiplyOperation();
    void GetHumanReadableMultiplier_ReturnsDivision_ForDivideOperation();
};

void CalcOperation_Tests::FactoryMethodOne_WillSetMultiplier()
{
    double testMultiplier = 1.1;
    ICalcOperation_shptr op = CreateCalcOperation(testMultiplier);

    double multiplier = op->GetMultiplier();

    QCOMPARE(multiplier, testMultiplier);
}

void CalcOperation_Tests::FactoryMethodTwo_WillSetMultiplier()
{
    double testMultiplier1 = 1.1;
    double testMultiplier2 = 2.2;
    ICalcOperation_shptr opFirst = CreateCalcOperation(testMultiplier1);
    ICalcOperation_shptr opSecond = CreateCalcOperation(testMultiplier2, opFirst);

    double multiplier = opSecond->GetMultiplier();

    QCOMPARE(multiplier, testMultiplier2);
}

void CalcOperation_Tests::FactoryMethodTwo_WillChainOperations()
{
    double testMultiplier1 = 1.1;
    double testMultiplier2 = 2.2;
    ICalcOperation_shptr opFirst = CreateCalcOperation(testMultiplier1);
    ICalcOperation_shptr opSecond = CreateCalcOperation(testMultiplier2, opFirst);

    double multiplier = opSecond->GetFinalMultiplier();

    QCOMPARE(multiplier, testMultiplier1 * testMultiplier2);
}

void CalcOperation_Tests::GetMultiplier_ReturnsPreviouslySetMultiplier()
{
    double testMultiplier = 2.3;

    ICalcOperation_shptr op = CreateCalcOperation(testMultiplier);

    QCOMPARE(op->GetMultiplier(), testMultiplier);
}

void CalcOperation_Tests::GetFinalMultiplier_ForNonChainedOperation_ReturnsShallowMultiplier()
{
    double testMultiplier = 2.3;

    ICalcOperation_shptr op = CreateCalcOperation(testMultiplier);

    QCOMPARE(op->GetFinalMultiplier(), testMultiplier);
}

void CalcOperation_Tests::GetFinalMultiplier_ForChainedOperations_ReturnsCumulativeMultiplier()
{
    double testMultiplier1 = 2.3;
    double testMultiplier2 = 0.4;

    ICalcOperation_shptr opFirst = CreateCalcOperation(testMultiplier1);
    ICalcOperation_shptr opSecond = CreateCalcOperation(testMultiplier2, opFirst);

    QCOMPARE(opSecond->GetFinalMultiplier(), testMultiplier1 * testMultiplier2);
}

void CalcOperation_Tests::SetPrevOperation_WillChainOperations()
{
    double testMultiplier1 = 1.1;
    double testMultiplier2 = 2.2;
    double testMultiplier3 = 3.3;
    ICalcOperation_shptr op1 = CreateCalcOperation(testMultiplier1);
    ICalcOperation_shptr op2 = CreateCalcOperation(testMultiplier2);
    ICalcOperation_shptr op3 = CreateCalcOperation(testMultiplier3);
    op2->SetPrevOperation(op1);
    op3->SetPrevOperation(op2);

    double cumulatedMultiplier = op3->GetFinalMultiplier();

    QCOMPARE(cumulatedMultiplier, testMultiplier1 * testMultiplier2 * testMultiplier3);
}

void CalcOperation_Tests::GetComplexityDistance_ForIdentityOperation_WillReturnZero()
{
    ICalcOperation_shptr opIdentity = CreateCalcOperation(1.);

    int complexity = opIdentity->GetComplexityDistance();

    QCOMPARE(complexity, 0);
}

void CalcOperation_Tests::GetComplexityDistance_ForSimpleOperation_WillReturnOne()
{
    double dummyMultiplier = 1.1;
    ICalcOperation_shptr op = CreateCalcOperation(dummyMultiplier);

    int complexity = op->GetComplexityDistance();

    QCOMPARE(complexity, 1);
}

void CalcOperation_Tests::GetComplexityDistance_ForChainedOperations_WillReturnNumberOfOperations()
{
    double testMultiplier1 = 1.1;
    double testMultiplier2 = 2.2;
    double testMultiplier3 = 3.3;
    ICalcOperation_shptr op1 = CreateCalcOperation(testMultiplier1);
    ICalcOperation_shptr op2 = CreateCalcOperation(testMultiplier2, op1);
    ICalcOperation_shptr op3 = CreateCalcOperation(testMultiplier3, op2);

    int complexity = op3->GetComplexityDistance();

    QCOMPARE(complexity, 3);
}

void CalcOperation_Tests::GetHumanReadableMultiplier_ReturnsOne_ForIdentityOperation()
{
    ICalcOperation_shptr op = CreateCalcOperation(1);

    QString formattedMathOperation = op->GetHumanReadableMultiplier();

    QCOMPARE(formattedMathOperation, QString("&#215;1"));
}

void CalcOperation_Tests::GetHumanReadableMultiplier_ReturnsMultiplication_ForMultiplyOperation()
{
    ICalcOperation_shptr op = CreateCalcOperation(2);

    QString formattedMathOperation = op->GetHumanReadableMultiplier();

    QCOMPARE(formattedMathOperation, QString("&#215;2"));
}

void CalcOperation_Tests::GetHumanReadableMultiplier_ReturnsDivision_ForDivideOperation()
{
    ICalcOperation_shptr op = CreateCalcOperation(.1);

    QString formattedMathOperation = op->GetHumanReadableMultiplier();

    QCOMPARE(formattedMathOperation, QString("&#247;10"));
}

#include "CalcOperation_Tests.moc"

DECLARE_TEST(CalcOperation_Tests)
