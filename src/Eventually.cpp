/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).  
 * Be sure to check out my books at www.bplearning.net!
 */

#include <Eventually.h>

/* *** EVT MANAGER *** */
EvtManager::EvtManager() {
  contextStack = new EvtContext[EVENTUALLY_MAX_CONTEXTS];
  contextStack[contextOffset].setupContext();
}

void EvtManager::addListener(EvtListener *lstn) {
  contextStack[contextOffset].addListener(lstn);
}

void EvtManager::removeListener(EvtListener *lstn) {
    contextStack[contextOffset].removeListener(lstn);
}

EvtContext *EvtManager::currentContext () {
  return &contextStack[contextOffset];
}

EvtContext *EvtManager::pushContext() {
  contextOffset++;
  contextStack[contextOffset].setupContext();
  return &contextStack[contextOffset];
}

EvtContext *EvtManager::resetContext() {
  contextStack[contextOffset].setupContext();
  return &contextStack[contextOffset];
}

EvtContext *EvtManager::popContext() {
  contextOffset--;
  return &contextStack[contextOffset];
}

void EvtManager::loopIteration() {
  contextStack[contextOffset].loopIteration();
}

/* *** EVT CONTEXT *** */

EvtContext::EvtContext() {
}

void EvtContext::loopIteration() {
  for(int i = 0; i < listenerCount; i++) {
    if(listeners[i]) { // Make sure it isn't deleted  
      if(listeners[i]->isEventTriggered()) { // If we are triggered, run the action
        if(listeners[i]->performTriggerAction(this)) { // If the action returns true, stop the chain
          return;
        }
      }
    }
  }
}

void EvtContext::setupContext() {
  if(data){
    delete data;
  }
  if(listeners) {
    for(int i = 0; i < listenerCount; i++) {
      if(listeners[i]) {
        delete listeners[i];
      }
    }
    delete listeners;
  }

  listeners = new EvtListener *[EVENTUALLY_MAX_LISTENERS];
  listenerCount = 0;
}
  
void EvtContext::addListener(EvtListener *lstn) {
  for(int i = 0; i < listenerCount; i++) { // Try to add in empty slot
    if(listeners[listenerCount] == 0) { 
      listeners[listenerCount] = lstn;
      return;
    }
  }

  // No empty slot, just add it
  listeners[listenerCount] = lstn;
  lstn->setupListener();
  listenerCount++;
}

void EvtContext::removeListener(EvtListener *lstn) {
  for(int i = 0; i < listenerCount; i++) {
    if(listeners[i] == lstn) {
      delete lstn;
      listeners[i] = 0;      
    }
  }
}

/* *** EVT LISTENER *** */

void EvtListener::setupListener() {
  
}

bool EvtListener::isEventTriggered() {
  return false;
}

bool EvtListener::performTriggerAction(EvtContext *ctx) {
  return (*triggerAction)(this, ctx);
}

/* *** EVT PIN LISTENER *** */

EvtPinListener::EvtPinListener() {
  
}

EvtPinListener::EvtPinListener(int pin, int debounce, bool targetValue, EvtAction action) {
  this->pin = pin;
  this->debounce = debounce;
  this->targetValue = targetValue;
  this->triggerAction = action;
}

EvtPinListener::EvtPinListener(int pin, int debounce, EvtAction action) {
  this->pin = pin;
  this->debounce = debounce;
  this->triggerAction = action;
}

EvtPinListener::EvtPinListener(int pin, EvtAction action) {
  this->pin = pin;
  this->triggerAction = action;
}

void EvtPinListener::setupListener() {
  startState = digitalRead(pin);
}

bool EvtPinListener::isEventTriggered() {
  bool val = digitalRead(pin); 

  // Debounce check if we were triggered earlier
  if(firstNoticed) {
    unsigned long curMillis = millis();
    if(curMillis > firstNoticed + debounce) {
      // Debounce time expired, check again

      // Reset Watcher
      firstNoticed = 0;

      // Check
      if(val == targetValue) {
        return true;
      } else {
        return false;
      }
    } else {
      // Waiting for debouncer to finish
      return false;
    }
  }
  
  if(mustStartOpposite && (startState == targetValue)) {
    /* This is a waiting loop to wait for the pin to change to the opposite state before sensing */
    /* Q - do I need to debounce mustStartOpposite? */
    if(val == startState) {
      // Do nothing
    } else {
      startState = val;
    }

    return false;
  } else {
    /* This is the real deal */
    if(val == targetValue) {
      if(debounce == 0) {
        return true;
      } else {
        firstNoticed = millis();
        return false;
      }
    } else {
      return false;
    }
  }
}

/* *** EVT TIME LISTENER *** */
EvtTimeListener::EvtTimeListener() {
  
}

EvtTimeListener::EvtTimeListener(unsigned long time, bool multiFire, EvtAction t) {
  this->millis = time;
  this->triggerAction = t;
  this->multiFire = multiFire;
}

void EvtTimeListener::setupListener() {
  startMillis = ::millis();
}

bool EvtTimeListener::isEventTriggered() {
  unsigned long curTime = ::millis();
  bool shouldFire = false;
  if(curTime >= startMillis) {
    /* Normal */
    if(curTime - startMillis > this->millis) {
      shouldFire = true;
    }
  } else {
    /* Wrap-Around! */
    if(((ULONG_MAX - startMillis) + curTime) > this->millis) {
      shouldFire = true;
    }
  }

  return shouldFire;  
}

bool EvtTimeListener::performTriggerAction(EvtContext *c) {
  bool returnval = (*triggerAction)(this, c);
  if(multiFire) {
    // On multifire, setup to receive the event again
    setupListener();
    // On multifire, we shouldn't stop the event chain no matter what, since we are just restarting in this context
    return false;
  } else {
    return returnval;
  }
}
