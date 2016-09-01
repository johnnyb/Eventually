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
  Serial.write("hello");
  int data = (int)l->extraData;
  Serial.write(data);
  if(data == 0) {
    data = 1;
  } else {
    data = 0;
  }  
  Serial.write(data);
  Serial.write("done");
  digitalWrite(LIGHT_PIN, data);
  l->extraData = (void *)data;
  return false;
}

bool set_speed() {
  mgr.resetContext();
  mgr.addListener(new EvtPinListener(BUTTON_PIN, (EvtAction)set_speed));
  speed = !speed; // Change speeds
  if(speed == HIGH) {
    mgr.addListener(new EvtTimeListener(250, true, (EvtAction)blink));
  } else {
    mgr.addListener(new EvtTimeListener(1000, true, (EvtAction)blink));
  }

  return true;
}

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  set_speed();
}

USE_EVENTUALLY_LOOP(mgr)
