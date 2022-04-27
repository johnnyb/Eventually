#include "StateMachineListener.h"

StateMachineListener::StateMachineListener()
{
}

void StateMachineListener::when(byte targetState, EvtAction action,
                                byte successState, byte failureState,
                                uint32_t timeout, bool repeatAction)
{
    StateAction s;
    s.action = action;
    s.state = targetState;
    s.successState = successState;
    s.failureState = failureState;
    s.timeout = timeout;
    s.repeatAction = repeatAction;

    _stateActions[targetState] = s;
}

bool StateMachineListener::performTriggerAction(EvtContext *ctx)
{
    byte currentState = _state;
    uint64_t currentTime = millis();

    StateAction s = _stateActions[currentState];

    if (currentTime - _transitionTime <= s.timeout)
    {
        return true;
    }

    bool result = (*s.action)(this, ctx);
    if (result)
    {
        if (s.successState != NO_TRANSITION)
        {
            _state = s.successState;
        }
    }
    else
    {
        _state = s.failureState;
    }

    if (_state != currentState)
    {
        setTransitionTime(currentTime);
    }

    return true;
}

void StateMachineListener::setTransitionTime(uint64_t timeInMs)
{
    _transitionTime = timeInMs;
}

void StateMachineListener::onInterrupt()
{
    if (currentState() == _interruptHandler.guardState)
    {
        setState(_interruptHandler.targetState);
    }
}

void StateMachineListener::whenInterrupted(byte guardState, byte targetState)
{
    _interruptHandler.guardState = guardState;
    _interruptHandler.targetState = targetState;
}

bool StateMachineListener::isEventTriggered()
{
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    return _state != STATE_FAILED;
}

void StateMachineListener::setState(byte state)
{
    _state = state;
}

byte StateMachineListener::currentState()
{
    return _state;
}

void StateMachineListener::setupListener()
{
}