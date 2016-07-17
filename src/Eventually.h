/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).  
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EVENTUALLY_H
#define EVENTUALLY_H

#include <limits.h>
#include <Arduino.h>

#define EVENTUALLY_MAX_CONTEXTS 10
#define EVENTUALLY_MAX_LISTENERS 20

class EvtManager;
class EvtContext;
class EvtListener;

typedef bool (*EvtAction)(EvtListener *, EvtContext *);

class EvtManager {

  public:

  EvtManager();
  void loopIteration();
  EvtContext *pushContext();
  EvtContext *resetContext();
  EvtContext *popContext();
  EvtContext *currentContext();
  void addListener(EvtListener *lstn);
  void removeListener(EvtListener *lstn);

  private:
  EvtContext *contextStack = 0;
  int contextOffset = 0;
  int contextDepth = 0;
};

// Note - should probably expand the number of available listeners by chaining contexts
class EvtContext {
  public:
  void *data = 0;

  EvtContext();
  void setupContext();
  void loopIteration();
  void addListener(EvtListener *lstn);
  void removeListener(EvtListener *lstn);

  private:
  EvtListener **listeners = 0;
  int listenerCount;
};


class EvtListener {
  public:
  void *extraData = 0; // Anything you want to store here
  EvtAction triggerAction;

  virtual void setupListener();
  virtual bool isEventTriggered();
  virtual bool performTriggerAction(EvtContext *); // return false if I should stop the current chain

  protected:
};

class EvtPinListener : public EvtListener {
  public:
  EvtPinListener();
  EvtPinListener(int pin, EvtAction trigger);
  EvtPinListener(int pin, int debounce, EvtAction action);
  int pin = 0;
  int debounce = 40;  
  bool targetValue = HIGH;
  bool mustStartOpposite = true;
  bool startState;
  unsigned long firstNoticed = 0;

  void setupListener();
  bool isEventTriggered();
};

class EvtTimeListener : public EvtListener {
  public:
  EvtTimeListener();
  EvtTimeListener(unsigned long time, bool multiFire, EvtAction trigger);
  unsigned long millis;
  void setupListener();
  bool isEventTriggered();
  bool performTriggerAction(EvtContext *);
  private:
  unsigned long startMillis;
  bool multiFire = false;
  int numFires = 0;
};

#define USE_EVENTUALLY_LOOP(mgr) void loop() { mgr.loopIteration(); }

#endif
