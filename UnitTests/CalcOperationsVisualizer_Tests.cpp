#include "AutoTest.h"

#include "src/MentolLib/ICalcOperation.h"
#include "src/MentolLib/ICalcOperationsVisualizer.h"

class CalcOperationsVisualizer_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void SingleSimpleOperation_Displayed_AsSingleEntryOnSingleLine();
    void SingleComplexOperation_Displayed_AsMultipleEntriesOnSingleLine();
    void MultipleSimpleOperations_Displayed_AsSingleEntriesOnMultipleLines();
    void MultipleComplexOperations_Displayed_AsMultipleEntriesOnMultipleLines();
};

void CalcOperationsVisualizer_Tests::SingleSimpleOperation_Displayed_AsSingleEntryOnSingleLine()
{
    std::unique_ptr<ICalcOperationsVisualizer> visualizer( CreateCalcOperationsTextualVisualizer() );
    ICalcOperation_shptr op = CreateCalcOperation(0.5);
    std::list<ICalcOperation_shptr> allOps;
    allOps.push_back(op);

    auto displayText = visualizer->GetResult(allOps);

    QCOMPARE(displayText, QString("0.5"));
}

void CalcOperationsVisualizer_Tests::SingleComplexOperation_Displayed_AsMultipleEntriesOnSingleLine()
{
    std::unique_ptr<ICalcOperationsVisualizer> visualizer( CreateCalcOperationsTextualVisualizer() );
    ICalcOperation_shptr subOp1 = CreateCalcOperation(0.1);
    ICalcOperation_shptr subOp2 = CreateCalcOperation(0.2, subOp1);
    ICalcOperation_shptr op = CreateCalcOperation(0.5, subOp2);
    std::list<ICalcOperation_shptr> allOps;
    allOps.push_back(op);

    auto displayText = visualizer->GetResult(allOps);

    QCOMPARE(displayText, QString("0.5 x 0.2 x 0.1"));
}

void CalcOperationsVisualizer_Tests::MultipleSimpleOperations_Displayed_AsSingleEntriesOnMultipleLines()
{
    std::unique_ptr<ICalcOperationsVisualizer> visualizer( CreateCalcOperationsTextualVisualizer() );
    ICalcOperation_shptr op1 = CreateCalcOperation(0.5);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.1);
    ICalcOperation_shptr op3 = CreateCalcOperation(10);
    std::list<ICalcOperation_shptr> allOps;
    allOps.push_back(op1);
    allOps.push_back(op2);
    allOps.push_back(op3);

    auto displayText = visualizer->GetResult(allOps);

    QCOMPARE(displayText, QString("0.5\n+ 0.1\n+ 10"));
}

void CalcOperationsVisualizer_Tests::MultipleComplexOperations_Displayed_AsMultipleEntriesOnMultipleLines()
{
    std::unique_ptr<ICalcOperationsVisualizer> visualizer( CreateCalcOperationsTextualVisualizer() );
    ICalcOperation_shptr subOp1a = CreateCalcOperation(2);
    ICalcOperation_shptr subOp1b = CreateCalcOperation(0.1, subOp1a);
    ICalcOperation_shptr op1 = CreateCalcOperation(0.5, subOp1b);
    ICalcOperation_shptr op2 = CreateCalcOperation(0.1);
    ICalcOperation_shptr subOp3a = CreateCalcOperation(0.5);
    ICalcOperation_shptr op3 = CreateCalcOperation(10, subOp3a);
    std::list<ICalcOperation_shptr> allOps;
    allOps.push_back(op1);
    allOps.push_back(op2);
    allOps.push_back(op3);

    auto displayText = visualizer->GetResult(allOps);

    QCOMPARE(displayText, QString("0.5 x 0.1 x 2\n+ 0.1\n+ 10 x 0.5"));
}

#include "CalcOperationsVisualizer_Tests.moc"

DECLARE_TEST(CalcOperationsVisualizer_Tests)

