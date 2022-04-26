#ifndef StateMachineListener_h
#define StateMachineListener_h

#include <Arduino.h>
#include "EvtListener.h"

const byte MAX_STATES = 10;
const byte NO_TRANSITION = -1;
const byte STATE_FAILED = -2;

struct stateAction
{
    EvtAction action;
    byte state;
    byte successState;
    byte failureState;
} typedef StateAction;

struct interruptHandler
{
    byte guardState;
    byte targetState;
} typedef InterruptHandler;

class StateMachineListener : public EvtListener
{
public:
    StateMachineListener();
    void setupListener();
    bool isEventTriggered();
    void when(byte targetState, EvtAction action,
                  byte successState = NO_TRANSITION, byte failureState = STATE_FAILED);
    void onInterrupt();
    void whenInterrupted(byte guardState, byte targetState);
    bool performTriggerAction(EvtContext *ctx);
    void setState(byte state);
    byte currentState();

private:
    volatile byte _state = 0;
    InterruptHandler _interruptHandler;
    StateAction _stateActions[MAX_STATES];
};

#endif