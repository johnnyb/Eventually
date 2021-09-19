#include "EvtByteListener.h"

EvtByteListener::EvtByteListener(byte *variable, byte targetValue, byte triggerMode, EvtAction action)
{
    _variable = variable;
    _targetValue = targetValue;
    _lastValue = targetValue + 1;
    triggerAction = action;
    triggerMode = triggerMode;
}

// EvtByteListener::EvtByteListener(byte *variable, byte targetValue, EvtAction action)
//     :  EvtByteListener(variable, targetValue, ON_CHANGE, triggerAction)
// {
// }

void EvtByteListener::setupListener()
{
    _lastValue = _targetValue + 1;
}

bool EvtByteListener::isEventTriggered()
{
    // byte lastValue = _lastValue;
    // _lastValue = *_variable;

    if (!EvtListener::isEventTriggered())
    {
        // Serial.print("NotEnabled");
        return false;
    }

    if (*_variable != _targetValue)
    {
        // Serial.print("NotTarget");
        // Serial.print(*_variable);
        return false;
    }

    // if (triggerMode == ON_CHANGE && *_variable == lastValue)
    // {
    //     Serial.print("AlreadyChanged");
    //     return false;
    // }

    return true;
}