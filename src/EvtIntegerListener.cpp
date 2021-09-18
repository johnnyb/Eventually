#include "EvtIntegerListener.h"

EvtIntegerListener::EvtIntegerListener(volatile int *variable, int targetValue, char triggerMode, EvtAction action)
{
    _variable = variable;
    _targetValue = targetValue;
    _lastValue = targetValue + 1;
    triggerAction = action;
    triggerMode = triggerMode;
}

EvtIntegerListener::EvtIntegerListener(volatile int *variable, int targetValue, EvtAction action)
    :  EvtIntegerListener(variable, targetValue, ON_CHANGE, triggerAction)
{
}

void EvtIntegerListener::setupListener()
{
    _lastValue = _targetValue + 1;
}

bool EvtIntegerListener::isEventTriggered()
{
    int lastValue = _lastValue;
    _lastValue = *_variable;

    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    if (*_variable != _targetValue)
    {
        return false;
    }

    if (triggerMode == ON_CHANGE && *_variable == lastValue)
    {
        return false;
    }

    return true;
}