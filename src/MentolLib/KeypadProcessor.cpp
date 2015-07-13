#include "KeypadProcessor.h"

const QString KeypadProcessor::zero("0");
const QString KeypadProcessor::point(".");
const QString KeypadProcessor::clear("C");
const QString KeypadProcessor::delChar("delChar");
const QString KeypadProcessor::ok("OK");
const QString KeypadProcessor::cancel("Cancel");

KeypadProcessor::KeypadProcessor()
{
    init();
}

KeypadProcessor::KeypadProcessor(std::function<void ()> onCancelCallback
                               , std::function<void ()> onOkCallback
                               , std::function<void ()> onChangeCallback)
{
    init();
    m_CancelCallback = onCancelCallback;
    m_OkCallback = onOkCallback;
    m_ChangeCallback = onChangeCallback;
}

void KeypadProcessor::OnKeyPressed(const QString& keyId)
{
    if (keyId == cancel)
    {
        m_CancelCallback();
        return;
    }

    if (keyId == ok)
    {
        m_OkCallback();
        return;
    }

    if (keyId == clear)
    {
        m_WholeInput = zero;
        m_ChangeCallback();
        return;
    }

    if (keyId == delChar)
    {
        if (m_WholeInput != KeypadProcessor::zero) // nothing to delete from "0"
        {
            auto withoutLastChar = m_WholeInput.left( m_WholeInput.length() - 1 );
            m_WholeInput = withoutLastChar;
            if (m_WholeInput.isEmpty()) // delete "1" to "0", rather than to ""
                m_WholeInput = zero;
        }
        m_ChangeCallback();
        return;
    }

    if (m_WholeInput == zero && keyId == zero)
        return; // use only a single zero digit for the zero number

    if (keyId == point && m_WholeInput.contains(point))
        return; // single decimal point allowed only

    if (m_WholeInput == zero && keyId != ".")
        m_WholeInput.clear(); // no leading zeroes allowed

    m_WholeInput += keyId;

    m_ChangeCallback();
}

QString KeypadProcessor::GetWholeInput() const
{
    return m_WholeInput;
}

void KeypadProcessor::init()
{
    m_WholeInput = zero;
    m_CancelCallback = [](){};
    m_OkCallback = [](){};
    m_ChangeCallback = [](){};
}
