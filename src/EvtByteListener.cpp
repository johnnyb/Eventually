#include "EvtByteListener.h"

EvtByteListener::EvtByteListener(volatile byte *variable, byte targetValue, EvtAction action)
{
    _variable = variable;
    _targetValue = targetValue;
    triggerAction = action;
}

void EvtByteListener::setupListener()
{
}

bool EvtByteListener::isEventTriggered()
{
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

    return true;
}