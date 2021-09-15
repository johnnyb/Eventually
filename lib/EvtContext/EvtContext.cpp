#include "EvtContext.h"

EvtContext::EvtContext()
{
}

void EvtContext::loopIteration()
{
    for (int i = 0; i < listenerCount; i++)
    {
        if (listeners[i])
        { // Make sure it isn't deleted
            if (listeners[i]->isEventTriggered())
            { // If we are triggered, run the action
                if (listeners[i]->performTriggerAction(this))
                { // If the action returns true, stop the chain
                    return;
                }
            }
        }
    }
}

void EvtContext::setupContext()
{
    if (data)
    {
        delete data;
    }
    if (listeners)
    {
        for (int i = 0; i < listenerCount; i++)
        {
            if (listeners[i])
            {
                delete listeners[i];
            }
        }
        delete listeners;
    }

    listeners = new EvtListener *[EVENTUALLY_MAX_LISTENERS];
    listenerCount = 0;
}

void EvtContext::addListener(EvtListener *lstn)
{
    for (int i = 0; i < listenerCount; i++)
    { // Try to add in empty slot
        if (listeners[listenerCount] == 0)
        {
            listeners[listenerCount] = lstn;
            return;
        }
    }

    // No empty slot, just add it
    listeners[listenerCount] = lstn;
    lstn->setupListener();
    listenerCount++;
}

void EvtContext::removeListener(EvtListener *lstn)
{
    for (int i = 0; i < listenerCount; i++)
    {
        if (listeners[i] == lstn)
        {
            delete lstn;
            listeners[i] = 0;
        }
    }
}