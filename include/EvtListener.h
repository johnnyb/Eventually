#ifndef EvtListener_h
#define EvtListener_h

#include "EvtAction.h"

class EvtContext;

class EvtListener
{
public:
    void *extraData = 0; // Anything you want to store here
    EvtAction triggerAction;
    bool enabled = true;

    virtual void setupListener();
    virtual bool isEventTriggered();
    virtual bool performTriggerAction(EvtContext *); // return false if I should stop the current chain

    virtual void disable();
    virtual void enable();

protected:
};

#endif