#ifndef EvtByteListener_h
#define EvtByteListener_h

#include <Arduino.h>
#include "EvtListener.h"

const byte ALWAYS = 0;
const byte ON_CHANGE = 1;

class EvtByteListener : public EvtListener
{
public:
    // EvtByteListener(byte *variable, byte targetValue, EvtAction action);
    EvtByteListener(byte *variable, byte targetValue, byte triggerMode, EvtAction action);
    void setupListener();
    bool isEventTriggered();
    byte triggerMode = ON_CHANGE;

private:
    byte *_variable;
    byte _targetValue;
    byte _lastValue;
};

#endif