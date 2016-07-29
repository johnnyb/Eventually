#include <Eventually.h>

/* 
 * This is the same as the "simple" example, except
 * that it shows how to store data in the listener itself.
 */

#define LIGHT_PIN 5
#define BUTTON_PIN 3

bool speed = LOW;
EvtManager mgr;

bool blink(EvtListener *l) {
  l->extraData = (void *)(!(bool)l->extraData); // Extra casting to keep C++ happy
  digitalWrite(LIGHT_PIN, (bool)l->extraData);
  return false;
}

bool set_speed() {
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTON_PIN, 50, (EvtAction)set_speed));
  speed = !speed; // Change speeds
  if(speed == HIGH) {
    mgr.addListener(new EvtTimeListener(LIGHT_PIN, 250, (EvtAction)blink));
  } else {
    mgr.addListener(new EvtTimeListener(LIGHT_PIN, 1000, (EvtAction)blink));
  }

  return true;
}

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  set_speed();
}

USE_EVENTUALLY_LOOP(mgr)
