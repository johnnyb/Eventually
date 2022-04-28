/*
 * This program is copyright 2016 by Jonathan Bartlett.  See LICENSING
 * file for information on usage (MIT License).  
 * Be sure to check out my books at www.bplearning.net!
 */

#ifndef EVENTUALLY_H
#define EVENTUALLY_H

#include <limits.h>

#include "EvtAction.h"
#include "EvtContext.h"
#include "EvtListener.h"
#include "EvtPinListener.h"
#include "EvtTimeListener.h"
#include "EvtByteListener.h"
#include "EvtManager.h"
#include "StateMachineListener.h"

#define USE_EVENTUALLY_LOOP(mgr) \
  void loop() { mgr.loopIteration(); }

#endif
