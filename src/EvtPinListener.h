/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EvtPinListener_h
#define EvtPinListener_h

#include <Arduino.h>
#include "EvtListener.h"

class EvtPinListener : public EvtListener
{
public:
  EvtPinListener();
  EvtPinListener(int pin, EvtAction trigger);
  EvtPinListener(int pin, int debounce, EvtAction action);
  EvtPinListener(int pin, int debounce, bool targetValue, EvtAction action);
  int pin = 0;
  int debounce = 40;
  bool targetValue = HIGH;
  bool mustStartOpposite = true;
  bool startState;
  unsigned long firstNoticed = 0;

  void setupListener();
  bool isEventTriggered();
};

#endif