#include "EvtTimeListener.h"

EvtTimeListener::EvtTimeListener()
{
}

EvtTimeListener::EvtTimeListener(unsigned long interval, bool multiFire, EvtAction triggerAction)
{
    _interval = interval;
    _triggerAction = triggerAction;
    _multiFire = multiFire;
}

void EvtTimeListener::reset()
{
    _startMillis = ::millis();
    _hasExecuted = false;
}

bool EvtTimeListener::isEventTriggered()
{
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    if (!_multiFire && _hasExecuted)
    {
        return false;
    }

    unsigned long curTime = ::millis();
    bool shouldFire = false;
    if (curTime >= _startMillis)
    {
        /* Normal */
        if (curTime - _startMillis > _interval)
        {
            shouldFire = true;
        }
    }
    else
    {
        /* Wrap-Around! */
        if (((ULONG_MAX - _startMillis) + curTime) > _interval)
        {
            shouldFire = true;
        }
    }

    return shouldFire;
}

bool EvtTimeListener::performTriggerAction(IEvtContext *c)
{
    bool returnVal = (*_triggerAction)(this, c);
    if (_multiFire)
    {
        // On multifire, setup to receive the event again
        reset();
        // On multifire, we shouldn't stop the event chain no matter what, since we are just restarting in this context
        return false;
    }
    else
    {
        _hasExecuted = true;
        return returnVal;
    }
}

void EvtTimeListener::setInterval(unsigned long interval)
{
    _interval = interval;
}