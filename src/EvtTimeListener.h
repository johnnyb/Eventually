/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtTimeListener_h
#define EvtTimeListener_h

#include <Arduino.h>
#include <limits.h>

#include "EvtListener.h"
#include "EvtContext.h"

class EvtTimeListener : public EvtListener
{
public:
    EvtTimeListener();
    EvtTimeListener(unsigned long time, bool multiFire, EvtAction trigger);
    unsigned long millis;
    void setupListener();
    bool isEventTriggered();
    bool performTriggerAction(EvtContext *);

private:
    unsigned long startMillis;
    bool multiFire = false;
    bool hasExecuted = false;
};

#endif