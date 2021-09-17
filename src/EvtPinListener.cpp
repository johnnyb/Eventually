#include "EvtPinListener.h"

EvtPinListener::EvtPinListener()
{
}

EvtPinListener::EvtPinListener(int pin, int debounce, bool targetValue, EvtAction action)
{
  this->pin = pin;
  this->debounce = debounce;
  this->targetValue = targetValue;
  this->triggerAction = action;
}

EvtPinListener::EvtPinListener(int pin, int debounce, EvtAction action)
{
  this->pin = pin;
  this->debounce = debounce;
  this->triggerAction = action;
}

EvtPinListener::EvtPinListener(int pin, EvtAction action)
{
  this->pin = pin;
  this->triggerAction = action;
}

void EvtPinListener::setupListener()
{
  startState = digitalRead(pin);
}

bool EvtPinListener::isEventTriggered()
{
  if (!EvtListener::isEventTriggered())
  {
    return false;
  }

  bool val = digitalRead(pin);

  // Debounce check if we were triggered earlier
  if (firstNoticed)
  {
    unsigned long curMillis = millis();
    if (curMillis > firstNoticed + debounce)
    {
      // Debounce time expired, check again

      // Reset Watcher
      firstNoticed = 0;

      // Check
      if (val == targetValue)
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

  if (mustStartOpposite && (startState == targetValue))
  {
    /* This is a waiting loop to wait for the pin to change to the opposite state before sensing */
    /* Q - do I need to debounce mustStartOpposite? */
    if (val == startState)
    {
      // Do nothing
    }
    else
    {
      startState = val;
    }

    return false;
  }
  else
  {
    /* This is the real deal */
    if (val == targetValue)
    {
      if (debounce == 0)
      {
        return true;
      }
      else
      {
        firstNoticed = millis();
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}