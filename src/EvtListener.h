/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

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