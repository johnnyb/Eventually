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
#include "IEvtContext.h"

class EvtTimeListener : public EvtListener
{
public:
    EvtTimeListener();
    EvtTimeListener(unsigned long interval, bool multiFire, EvtAction trigger);
    void reset();
    bool isEventTriggered();
    bool performTriggerAction(IEvtContext *);
    void setInterval(unsigned long interval);

private:
    unsigned long _interval;
    unsigned long _startMillis;
    bool _multiFire = false;
    bool _hasExecuted = false;
};

#endif