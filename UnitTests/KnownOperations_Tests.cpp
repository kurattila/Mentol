#include "AutoTest.h"

#include "src/MentolLib/IKnownOperations.h"

class KnownOperations_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void FactoryMethodOne_CreatesPopulatedCollection();
    void FactoryMethodTwo_CreatesEmptyCollection();
    void AddBaseOperation_WillAddOperation();
    void AddBaseOperation_ForDefaultComplexity_WontAddDerivedOperations();
    void AddBaseOperation_ForLevelTwoComplexity_WillAddDerivedOperations();
    void AddBaseOperation_ForLevelThreeComplexity_WillAddDerivedOperations();
};

void KnownOperations_Tests::FactoryMethodOne_CreatesPopulatedCollection()
{
    std::unique_ptr<IKnownOperations> knownOperations(CreateKnownOperationsCollection());

    size_t operationsCount = knownOperations->GetAllKnownOperations().size();

    QCOMPARE(operationsCount, 5UL);
}

void KnownOperations_Tests::FactoryMethodTwo_CreatesEmptyCollection()
{
    std::unique_ptr<IKnownOperations> knownOperations(CreateKnownOperationsEmptyCollection());

    size_t operationsCount = knownOperations->GetAllKnownOperations().size();

    QCOMPARE(operationsCount, 0UL);
}

void KnownOperations_Tests::AddBaseOperation_WillAddOperation()
{
    std::shared_ptr<IKnownOperations> knownOperations(CreateKnownOperationsEmptyCollection());

    knownOperations->AddBaseOperation(CreateCalcOperation(1.1));

    QCOMPARE(knownOperations->GetAllKnownOperations().size(), 1UL);
}

void KnownOperations_Tests::AddBaseOperation_ForDefaultComplexity_WontAddDerivedOperations()
{
    std::shared_ptr<IKnownOperations> knownOperations(CreateKnownOperationsEmptyCollection());

    knownOperations->AddBaseOperation(CreateCalcOperation(1.1));
    knownOperations->AddBaseOperation(CreateCalcOperation(2.2));
    knownOperations->AddBaseOperation(CreateCalcOperation(3.3));

    QCOMPARE(knownOperations->GetAllKnownOperations().size(), 3UL);
}

void KnownOperations_Tests::AddBaseOperation_ForLevelTwoComplexity_WillAddDerivedOperations()
{
    std::shared_ptr<IKnownOperations> knownOperations(CreateKnownOperationsEmptyCollection());
    knownOperations->SetMaxComplexity(2);

    knownOperations->AddBaseOperation(CreateCalcOperation(1.1));
    knownOperations->AddBaseOperation(CreateCalcOperation(2.2));
    knownOperations->AddBaseOperation(CreateCalcOperation(3.3));

    QCOMPARE(knownOperations->GetAllKnownOperations().size(), 9UL);
}

void KnownOperations_Tests::AddBaseOperation_ForLevelThreeComplexity_WillAddDerivedOperations()
{
    std::shared_ptr<IKnownOperations> knownOperations(CreateKnownOperationsEmptyCollection());
    knownOperations->SetMaxComplexity(3);

    knownOperations->AddBaseOperation(CreateCalcOperation(1.1));
    knownOperations->AddBaseOperation(CreateCalcOperation(2.2));
    knownOperations->AddBaseOperation(CreateCalcOperation(3.3));

//    1.1
//    2.2
//    3.3
//    1.1 * 1.1
//    1.1 * 2.2
//    1.1 * 3.3
//    2.2 * 2.2
//    2.2 * 3.3
//    3.3 * 3.3
//    1.1 * 1.1 * 1.1
//    1.1 * 1.1 * 2.2
//    1.1 * 1.1 * 3.3
//    1.1 * 2.2 * 2.2
//    1.1 * 2.2 * 3.3
//    1.1 * 3.3 * 3.3
//    2.2 * 2.2 * 2.2
//    2.2 * 2.2 * 3.3
//    2.2 * 3.3 * 3.3
//    3.3 * 3.3 * 3.3

    QCOMPARE(knownOperations->GetAllKnownOperations().size(), 19UL);
}

#include "KnownOperations_Tests.moc"

DECLARE_TEST(KnownOperations_Tests)

