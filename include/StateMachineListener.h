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
    uint32_t transitionDelay;
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
              byte successState = NO_TRANSITION,
              byte failureState = STATE_FAILED,
              uint32_t transitionDelay = 0);
    void onInterrupt();
    void whenInterrupted(byte guardState, byte targetState);
    bool performTriggerAction(EvtContext *ctx);
    void transition(byte newState);
    byte currentState();
    void setTransitionTime(uint64_t timeInMs);
    uint64_t transitionTime();
    uint64_t systemTime();

private:
    volatile byte _state = 0;
    InterruptHandler _interruptHandler;
    StateAction _stateActions[MAX_STATES];
    uint64_t _transitionTime;
    bool _actionExecuted;
    bool hasPassedTransitionDelay(uint32_t transitionDelay);
};

#endif