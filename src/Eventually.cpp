/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).  
 * Be sure to check out my books at www.bplearning.net!
 */

#include <Eventually.h>

/* *** EVT MANAGER *** */
EvtManager::EvtManager() {
  for(int i = 0; i < EVENTUALLY_CONTEXT_BUFFER_SIZE; i++) {
    contextStack[i] = NULL;
  }
  setupCurrentContext();
}


void EvtManager::setupCurrentContext() {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		nextManager->setupCurrentContext();
	} else {
		if(contextStack[contextOffset] == NULL) {
			contextStack[contextOffset] = new EvtContext();
		}
		contextStack[contextOffset]->setupContext();
	}
}

void EvtManager::addListener(EvtListener *lstn) {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		nextManager->addListener(lstn);
	} else {
		contextStack[contextOffset]->addListener(lstn);
	}
}

void EvtManager::removeListener(EvtListener *lstn) {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		nextManager->removeListener(lstn);
	} else {
		contextStack[contextOffset]->removeListener(lstn);
	}
}

EvtContext *EvtManager::currentContext () {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		return nextManager->currentContext();
	} else {
		return contextStack[contextOffset];
	}
}

EvtContext *EvtManager::pushContext() {
	contextOffset++;
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		if(!nextManager) {
			nextManager = new EvtManager();
		}
		return nextManager->pushContext();
	} else {
		if(contextStack[contextOffset] == NULL) {
			contextStack[contextOffset] = new EvtContext();
		}
		contextStack[contextOffset]->setupContext();
		return contextStack[contextOffset];
	}
}

EvtContext *EvtManager::resetContext() {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		return nextManager->resetContext();
	} else {
		if(contextStack[contextOffset] == NULL) {
			contextStack[contextOffset] = new EvtContext();
		}
		contextStack[contextOffset]->setupContext();
		return contextStack[contextOffset];
	}
}

EvtContext *EvtManager::popContext() {
	contextOffset--;
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		return nextManager->popContext();
	} else if(contextOffset == (EVENTUALLY_CONTEXT_BUFFER_SIZE - 1)) {
		nextManager->popContext();
		return contextStack[contextOffset];
	} else if(contextOffset == -1) {
		return NULL;
	} else {
		return contextStack[contextOffset];
	}
}

void EvtManager::loopIteration() {
	if(contextOffset >= EVENTUALLY_CONTEXT_BUFFER_SIZE) {
		nextManager->loopIteration();
	} else {
		contextStack[contextOffset]->loopIteration();
	}
}

/* *** EVT CONTEXT *** */

EvtContext::EvtContext() {
	for(int i = 0; i < EVENTUALLY_LISTENER_BUFFER_SIZE; i++) {
		listeners[i] = NULL;
	}
}

EvtContext::~EvtContext() {
	setupContext();
}

void EvtContext::loopIteration() {
  for(int i = 0; i < EVENTUALLY_LISTENER_BUFFER_SIZE; i++) {
    if(listeners[i]) { // Make sure it isn't deleted  
      if(listeners[i]->isEventTriggered()) { // If we are triggered, run the action
        if(listeners[i]->performTriggerAction(this)) { // If the action returns true, stop the chain
          return;
        }
      }
    }
  }
  if(nextContext) {
    nextContext->loopIteration();
  }
}

void EvtContext::setupContext() {
  if(data){
    delete data;
  }
  if(nextContext) {
    delete nextContext;
    nextContext = NULL;
  }
  if(listeners) {
    for(int i = 0; i < EVENTUALLY_LISTENER_BUFFER_SIZE; i++) {
      if(listeners[i]) {
        delete listeners[i];
	listeners[i] = NULL;
      }
    }
  }
}
  
void EvtContext::addListener(EvtListener *lstn) {
  for(int i = 0; i < EVENTUALLY_LISTENER_BUFFER_SIZE; i++) { // Try to add in empty slot
    if(listeners[i] == NULL) { 
      listeners[i] = lstn;
      lstn->setupListener();
      return;
    }
  }
  if(!nextContext) {
    nextContext = new EvtContext();
  }
  nextContext->addListener(lstn);
}

void EvtContext::removeListener(EvtListener *lstn) {
  for(int i = 0; i < EVENTUALLY_LISTENER_BUFFER_SIZE; i++) {
    if(listeners[i] == lstn) {
      delete lstn;
      listeners[i] = NULL;
      return;
    }
  }
  if(nextContext) {
    nextContext->removeListener(lstn);
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
