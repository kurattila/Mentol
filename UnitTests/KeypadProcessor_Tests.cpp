#include "AutoTest.h"

#include "src/MentolLib/KeypadProcessor.h"

class KeypadProcessor_Tests : public QObject
{
    Q_OBJECT
public:

private slots:
    void GetWholeInput_ReturnsZero_Initially();
    void GetWholeInput_ReturnsZero_InitiallyWhenCallbacksSpecified();
    void GetWholeInput_WontContainOkKeypress_Always();
    void GetWholeInput_WontContainCancelKeypress_Always();

    void OnKeyPressed_AddsDigitKeyToWholeInput_WhenInputEmpty();
    void OnKeyPressed_AddsDigitKeyToWholeInput_WhenInputNotEmpty();
    void OnKeyPressed_AddsNothingToWholeInput_WhenInputEmptyAndZeroPressed();
    void OnKeyPressed_AddsZeroToWholeInput_WhenInputNotEmptyAndZeroPressed();
    void OnKeyPressed_AddsPointToWholeInput_WhenOnlyWholeNumberDigitsEnteredYet();
    void OnKeyPressed_AddsDigitKeyToWholeInput_WhenPointAlreadyEnteredPreviously();
    void OnKeyPressed_WontAddPointToWholeInput_WhenPointAlreadyEnteredPreviously();
    void OnKeyPressed_ClearsWholeInputCompletely_WhenClearPressed();
    void OnKeyPressed_ClearsTheOnlyDigit_WhenDelCharPressed();
    void OnKeyPressed_ClearsLastDigit_WhenDelCharPressed();
    void OnKeyPressed_ClearsDecimalPoint_WhenDelCharPressed();
    void OnKeyPressed_WontClearAnything_WhenDelCharPressedAndAlreadyEmpty();

    void OnKeyPressed_WillCalculateReciprocalValue_WhenOnePerXPressed();
    void OnKeyPressed_DoesNothing_WhenOnePerXPressedForZeroValue();

    void OnKeyPressed_CallsOkCallback_WhenOkPressed();
    void OnKeyPressed_CallsCancelCallback_WhenCancelPressed();
    void OnKeyPressed_WontCallOkCallback_WhenOkNotPressed();
    void OnKeyPressed_WontCallCancelCallback_WhenCancelNotPressed();
    void OnKeyPressed_WontCallOkCallback_WhenCancelPressed();
    void OnKeyPressed_WontCallCancelCallback_WhenOkPressed();

    void OnKeyPressed_WillNotifyAboutChange_WhenDigitPressed();
    void OnKeyPressed_WillNotifyAboutChange_WhenClearPressed();
    void OnKeyPressed_WillNotifyAboutChange_WhenDelCharPressed();
    void OnKeyPressed_WillNotifyAboutChange_WhenOnePerXPressed();
    void OnKeyPressed_WillNotifyAboutDigitChange_OnlyAfterChangeHasOccurred();
    void OnKeyPressed_WillNotifyAboutClearChange_OnlyAfterChangeHasOccurred();
    void OnKeyPressed_WillNotifyAboutDelCharChange_OnlyAfterChangeHasOccurred();
};

void KeypadProcessor_Tests::GetWholeInput_ReturnsZero_Initially()
{
    KeypadProcessor processor;

    QString wholeInput = processor.GetWholeInput();

    QCOMPARE(wholeInput, QString("0"));
}

void KeypadProcessor_Tests::GetWholeInput_ReturnsZero_InitiallyWhenCallbacksSpecified()
{
    KeypadProcessor processor([](){}
                            , [](){}
                            , [](){});

    QString wholeInput = processor.GetWholeInput();

    QCOMPARE(wholeInput, QString("0"));
}

void KeypadProcessor_Tests::GetWholeInput_WontContainOkKeypress_Always()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");

    processor.OnKeyPressed(KeypadProcessor::ok);

    QCOMPARE(processor.GetWholeInput(), QString("1"));
}

void KeypadProcessor_Tests::GetWholeInput_WontContainCancelKeypress_Always()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");

    processor.OnKeyPressed(KeypadProcessor::cancel);

    QCOMPARE(processor.GetWholeInput(), QString("1"));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsDigitKeyToWholeInput_WhenInputEmpty()
{
    KeypadProcessor processor;

    processor.OnKeyPressed("1");

    QCOMPARE(processor.GetWholeInput(), QString("1"));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsDigitKeyToWholeInput_WhenInputNotEmpty()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("2");

    processor.OnKeyPressed("1");

    QCOMPARE(processor.GetWholeInput(), QString("21"));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsNothingToWholeInput_WhenInputEmptyAndZeroPressed()
{
    KeypadProcessor processor;

    processor.OnKeyPressed("0");

    QCOMPARE(processor.GetWholeInput(), QString("0"));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsZeroToWholeInput_WhenInputNotEmptyAndZeroPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("2");

    processor.OnKeyPressed("0");

    QCOMPARE(processor.GetWholeInput(), QString("20"));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsPointToWholeInput_WhenOnlyWholeNumberDigitsEnteredYet()
{
    KeypadProcessor processor;

    processor.OnKeyPressed(KeypadProcessor::point);

    QCOMPARE(processor.GetWholeInput(), QString("0."));
}

void KeypadProcessor_Tests::OnKeyPressed_AddsDigitKeyToWholeInput_WhenPointAlreadyEnteredPreviously()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");
    processor.OnKeyPressed("2");
    processor.OnKeyPressed(KeypadProcessor::point);

    processor.OnKeyPressed("9");

    QCOMPARE(processor.GetWholeInput(), QString("12.9"));
}

void KeypadProcessor_Tests::OnKeyPressed_WontAddPointToWholeInput_WhenPointAlreadyEnteredPreviously()
{
    KeypadProcessor processor;
    processor.OnKeyPressed(KeypadProcessor::point);
    processor.OnKeyPressed("1");
    processor.OnKeyPressed(KeypadProcessor::point);

    QCOMPARE(processor.GetWholeInput(), QString("0.1"));
}

void KeypadProcessor_Tests::OnKeyPressed_ClearsWholeInputCompletely_WhenClearPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");
    processor.OnKeyPressed("2");
    processor.OnKeyPressed(KeypadProcessor::point);
    processor.OnKeyPressed("9");

    processor.OnKeyPressed(KeypadProcessor::clear);

    QCOMPARE(processor.GetWholeInput(), QString("0"));

}

void KeypadProcessor_Tests::OnKeyPressed_ClearsTheOnlyDigit_WhenDelCharPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(processor.GetWholeInput(), QString("0"));
}

void KeypadProcessor_Tests::OnKeyPressed_ClearsLastDigit_WhenDelCharPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");
    processor.OnKeyPressed("2");
    processor.OnKeyPressed("3");

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(processor.GetWholeInput(), QString("12"));
}

void KeypadProcessor_Tests::OnKeyPressed_ClearsDecimalPoint_WhenDelCharPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("1");
    processor.OnKeyPressed("2");
    processor.OnKeyPressed(KeypadProcessor::point);

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(processor.GetWholeInput(), QString("12"));
}

void KeypadProcessor_Tests::OnKeyPressed_WontClearAnything_WhenDelCharPressedAndAlreadyEmpty()
{
    KeypadProcessor processor;

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(processor.GetWholeInput(), QString("0"));
}

void KeypadProcessor_Tests::OnKeyPressed_WillCalculateReciprocalValue_WhenOnePerXPressed()
{
    KeypadProcessor processor;
    processor.OnKeyPressed("2");
    processor.OnKeyPressed("0");

    processor.OnKeyPressed(KeypadProcessor::onePerX);

    QCOMPARE(processor.GetWholeInput(), QString("0.05"));
}

void KeypadProcessor_Tests::OnKeyPressed_DoesNothing_WhenOnePerXPressedForZeroValue()
{
    KeypadProcessor processor;

    processor.OnKeyPressed(KeypadProcessor::onePerX);

    QCOMPARE(processor.GetWholeInput(), QString("0"));
}

void KeypadProcessor_Tests::OnKeyPressed_CallsOkCallback_WhenOkPressed()
{
    bool okCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [&](){ okCallbackCalled = true; }
                            , [](){});

    processor.OnKeyPressed(KeypadProcessor::ok);

    QCOMPARE(okCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_CallsCancelCallback_WhenCancelPressed()
{
    bool cancelCallbackCalled = false;
    KeypadProcessor processor([&](){ cancelCallbackCalled = true; }
                            , [](){}
                            , [](){});

    processor.OnKeyPressed(KeypadProcessor::cancel);

    QCOMPARE(cancelCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_WontCallOkCallback_WhenOkNotPressed()
{
    bool okCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [&](){ okCallbackCalled = true; }
                            , [](){});

    processor.OnKeyPressed("1");

    QCOMPARE(okCallbackCalled, false);
}

void KeypadProcessor_Tests::OnKeyPressed_WontCallCancelCallback_WhenCancelNotPressed()
{
    bool cancelCallbackCalled = false;
    KeypadProcessor processor([&](){ cancelCallbackCalled = true; }
                            , [](){}
                            , [](){});

    processor.OnKeyPressed("1");

    QCOMPARE(cancelCallbackCalled, false);
}

void KeypadProcessor_Tests::OnKeyPressed_WontCallOkCallback_WhenCancelPressed()
{
    bool okCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [&](){ okCallbackCalled = true; }
                            , [](){});

    processor.OnKeyPressed(KeypadProcessor::cancel);

    QCOMPARE(okCallbackCalled, false);
}

void KeypadProcessor_Tests::OnKeyPressed_WontCallCancelCallback_WhenOkPressed()
{
    bool cancelCallbackCalled = false;
    KeypadProcessor processor([&](){ cancelCallbackCalled = true; }
                            , [](){}
                            , [](){});

    processor.OnKeyPressed(KeypadProcessor::ok);

    QCOMPARE(cancelCallbackCalled, false);
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutChange_WhenDigitPressed()
{
    bool changeCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ changeCallbackCalled = true; });

    processor.OnKeyPressed("1");

    QCOMPARE(changeCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutChange_WhenClearPressed()
{
    bool changeCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ changeCallbackCalled = true; });

    processor.OnKeyPressed(KeypadProcessor::clear);

    QCOMPARE(changeCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutChange_WhenDelCharPressed()
{
    bool changeCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ changeCallbackCalled = true; });

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(changeCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutChange_WhenOnePerXPressed()
{
    bool changeCallbackCalled = false;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ changeCallbackCalled = true; });
    processor.OnKeyPressed("1");
    changeCallbackCalled = false;

    processor.OnKeyPressed(KeypadProcessor::onePerX);

    QCOMPARE(changeCallbackCalled, true);
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutDigitChange_OnlyAfterChangeHasOccurred()
{
    QString wholeInputOnChangeNotification;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ wholeInputOnChangeNotification = processor.GetWholeInput(); });

    processor.OnKeyPressed("1");

    QCOMPARE(wholeInputOnChangeNotification, QString("1"));
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutClearChange_OnlyAfterChangeHasOccurred()
{
    QString wholeInputOnChangeNotification;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ wholeInputOnChangeNotification = processor.GetWholeInput(); });
    processor.OnKeyPressed("1");

    processor.OnKeyPressed(KeypadProcessor::clear);

    QCOMPARE(wholeInputOnChangeNotification, QString("0"));
}

void KeypadProcessor_Tests::OnKeyPressed_WillNotifyAboutDelCharChange_OnlyAfterChangeHasOccurred()
{
    QString wholeInputOnChangeNotification;
    KeypadProcessor processor([](){}
                            , [](){}
                            , [&](){ wholeInputOnChangeNotification = processor.GetWholeInput(); });
    processor.OnKeyPressed("1");
    processor.OnKeyPressed("2");

    processor.OnKeyPressed(KeypadProcessor::delChar);

    QCOMPARE(wholeInputOnChangeNotification, QString("1"));
}

#include "KeypadProcessor_Tests.moc"

DECLARE_TEST(KeypadProcessor_Tests)

