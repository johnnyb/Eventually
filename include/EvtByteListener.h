#ifndef EvtByteListener_h
#define EvtByteListener_h

#include <Arduino.h>
#include "EvtListener.h"

class EvtByteListener : public EvtListener
{
public:
    EvtByteListener(volatile byte *variable, byte targetValue, EvtAction action);
    void setupListener();
    bool isEventTriggered();

private:
    volatile byte *_variable;
    byte _targetValue;
    byte _lastValue;
};

#endif