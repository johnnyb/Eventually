#include "EvtIntegerListener.h"

EvtIntegerListener::EvtIntegerListener(int *variable, int targetValue, EvtAction action)
{
    _variable = variable;
    _targetValue = targetValue;
    _lastValue = targetValue + 1;
    triggerAction = action;
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

    if (*_variable == lastValue)
    {
        return false;
    }

    return true;
}