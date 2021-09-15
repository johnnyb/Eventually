#ifndef EvtTimeListener_h
#define EvtTimeListener_h

#include <Arduino.h>

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
    int numFires = 0;
};

#endif