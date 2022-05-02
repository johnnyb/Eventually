#include "EvtTimeListener.h"

EvtTimeListener::EvtTimeListener()
{
}

EvtTimeListener::EvtTimeListener(unsigned long time, bool multiFire, EvtAction t)
{
    this->millis = time;
    this->triggerAction = t;
    this->multiFire = multiFire;
}

void EvtTimeListener::setupListener()
{
    startMillis = ::millis();
    hasExecuted = false;
}

bool EvtTimeListener::isEventTriggered()
{
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    if (!multiFire && hasExecuted)
    {
        return false;
    }

    unsigned long curTime = ::millis();
    bool shouldFire = false;
    if (curTime >= startMillis)
    {
        /* Normal */
        if (curTime - startMillis > this->millis)
        {
            shouldFire = true;
        }
    }
    else
    {
        /* Wrap-Around! */
        if (((ULONG_MAX - startMillis) + curTime) > this->millis)
        {
            shouldFire = true;
        }
    }

    return shouldFire;
}

bool EvtTimeListener::performTriggerAction(EvtContext *c)
{
    bool returnval = (*triggerAction)(this, c);
    if (multiFire)
    {
        // On multifire, setup to receive the event again
        setupListener();
        // On multifire, we shouldn't stop the event chain no matter what, since we are just restarting in this context
        return false;
    }
    else
    {
        hasExecuted = true;
        return returnval;
    }
}
