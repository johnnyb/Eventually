#ifndef EvtIntegerListener_h
#define EvtIntegerListener_h

#include "EvtListener.h"

const char ALWAYS = 0;
const char ON_CHANGE = 1;

class EvtIntegerListener : public EvtListener
{
public:
    EvtIntegerListener(volatile int *variable, int targetValue, EvtAction action);
    EvtIntegerListener(volatile int *variable, int targetValue, char triggerMode, EvtAction action);
    void setupListener();
    bool isEventTriggered();
    char triggerMode = ON_CHANGE;

private:
    volatile int *_variable;
    int _targetValue;
    int _lastValue;
};

#endif