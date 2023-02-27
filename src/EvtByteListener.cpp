#include "EvtByteListener.h"

EvtByteListener::EvtByteListener(volatile byte *variable, byte targetValue, EvtAction action)
{
    _variable = variable;
    _targetValue = targetValue;
    _triggerAction = action;
}

void EvtByteListener::reset()
{
}

bool EvtByteListener::isEventTriggered()
{
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    if (*_variable != _targetValue)
    {
        return false;
    }

    return true;
}

EvtByteListener::~EvtByteListener()
{
}
