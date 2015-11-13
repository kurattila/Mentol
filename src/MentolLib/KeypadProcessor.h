#ifndef KEYPADPROCESSOR_H
#define KEYPADPROCESSOR_H

#include <QString>
#include <functional>

class KeypadProcessor
{
public:
    KeypadProcessor();
    KeypadProcessor(std::function<void()> onCancelCallback
                  , std::function<void()> onOkCallback
                  , std::function<void()> onChangeCallback);

    virtual void OnKeyPressed(const QString& keyId);
    virtual QString GetWholeInput() const;

    static const QString zero;
    static const QString point;
    static const QString clear;
    static const QString delChar;
    static const QString ok;
    static const QString cancel;
    static const QString onePerX;

private:
    void init();

    std::function<void()> m_CancelCallback;
    std::function<void()> m_OkCallback;
    std::function<void()> m_ChangeCallback;
    QString m_WholeInput;
};

#endif // KEYPADPROCESSOR_H
