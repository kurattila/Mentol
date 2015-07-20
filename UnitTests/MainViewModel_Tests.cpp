#include "AutoTest.h"

#include "src/MentolLib/MainViewModel.h"

class MainViewModel_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void SetUserPreferenceOfComplexity_WillCallUpdate_ByDefault();
    void SetUserPreferenceOfComplexity_WillCallUpdate_EvenIfComplexityDistributionCompletelyEmpty();
};


class FakeMainViewModel : public MainViewModel
{
public:
    FakeMainViewModel()
        : UpdateHasBeenCalled(false)
    { }
    bool UpdateHasBeenCalled;

protected:
    virtual void updateOperationsVisualization(int tolerancePercents) override
    {
        MainViewModel::updateOperationsVisualization(tolerancePercents);
        UpdateHasBeenCalled = true;
    }
};


void MainViewModel_Tests::SetUserPreferenceOfComplexity_WillCallUpdate_ByDefault()
{
    auto spyVm = new FakeMainViewModel();
    spyVm->calculate(1.4);

    spyVm->setUserPreferenceOfComplexity(15);

    QVERIFY(spyVm->UpdateHasBeenCalled);
}

void MainViewModel_Tests::SetUserPreferenceOfComplexity_WillCallUpdate_EvenIfComplexityDistributionCompletelyEmpty()
{
    auto spyVm = new FakeMainViewModel();
    spyVm->calculate(0.00001); // an impossibly difficult operation will have no solution, thus empty distribution

    spyVm->setUserPreferenceOfComplexity(15);

    QVERIFY(spyVm->UpdateHasBeenCalled);
}

#include "MainViewModel_Tests.moc"

DECLARE_TEST(MainViewModel_Tests)
