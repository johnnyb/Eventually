#include "EvtPinListener.h"

EvtPinListener::EvtPinListener()
{
}

EvtPinListener::EvtPinListener(byte pin, int debounce, bool targetValue, EvtAction action)
{
  _pin = pin;
  _debounce = debounce;
  _targetValue = targetValue;
  _triggerAction = action;
}

EvtPinListener::EvtPinListener(byte pin, int debounce, EvtAction action)
{
  _pin = pin;
  _debounce = debounce;
  _triggerAction = action;
}

EvtPinListener::EvtPinListener(byte pin, EvtAction action)
{
  _pin = pin;
  _triggerAction = action;
}

void EvtPinListener::reset()
{
  _startState = digitalRead(_pin);
}

bool EvtPinListener::isEventTriggered()
{
  if (!EvtListener::isEventTriggered())
  {
    return false;
  }

  bool val = digitalRead(_pin);

  // Debounce check if we were triggered earlier
  if (_firstNoticed)
  {
    unsigned long curMillis = millis();
    if (curMillis > _firstNoticed + _debounce)
    {
      // Debounce time expired, check again

      // Reset Watcher
      _firstNoticed = 0;

      // Check
      if (val == _targetValue)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      // Waiting for debouncer to finish
      return false;
    }
  }

  if (_mustStartOpposite && (_startState == _targetValue))
  {
    /* This is a waiting loop to wait for the pin to change to the opposite state before sensing */
    /* Q - do I need to debounce mustStartOpposite? */
    if (val == _startState)
    {
      // Do nothing
    }
    else
    {
      _startState = val;
    }

    return false;
  }
  else
  {
    /* This is the real deal */
    if (val == _targetValue)
    {
      if (_debounce == 0)
      {
        return true;
      }
      else
      {
        _firstNoticed = millis();
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}