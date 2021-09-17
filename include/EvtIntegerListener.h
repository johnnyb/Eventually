#ifndef EvtIntegerListener_h
#define EvtIntegerListener_h

#include "EvtListener.h"

class EvtIntegerListener : public EvtListener
{
public:
    EvtIntegerListener(int *variable, int targetValue, EvtAction action);
    void setupListener();
    bool isEventTriggered();

private:
    int *_variable;
    int _targetValue;
    int _lastValue;
};

#endif