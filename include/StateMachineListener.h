#ifndef StateMachineListener_h
#define StateMachineListener_h

#include <Arduino.h>
#include "EvtListener.h"

const byte MAX_STATES = 10;

struct stateAction
{
    EvtAction action;
    byte state;
    byte successState;
    byte failureState;
} typedef StateAction;

class StateMachineListener : public EvtListener
{
public:
    StateMachineListener();
    void setupListener();
    void addState(byte targetState, EvtAction action,
                  byte successState, byte failureState = 0);
    bool performTriggerAction(EvtContext *ctx);
    void setState(byte state);
    byte currentState();

private:
    volatile byte _state = 0;
    StateAction _stateActions[MAX_STATES];
};

#endif