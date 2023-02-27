#ifndef EvtByteListener_h
#define EvtByteListener_h

#include "Common.h"
#include "EvtListener.h"

class EvtByteListener : public EvtListener
{
public:
    EvtByteListener(volatile byte *variable, byte targetValue, EvtAction action);
    void reset();
    bool isEventTriggered();
    ~EvtByteListener();

private:
    volatile byte *_variable;
    byte _targetValue;
    byte _lastValue;
};

#endif