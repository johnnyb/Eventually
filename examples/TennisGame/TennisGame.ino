#include <Eventually.h>

/*
 * This is a simple Tennis game using the Eventually library.
 * To use, wire up two buttons (one for each player, and then
 * a series of lights to be used for the ball.  You can
 * change PLAYER_BUTTON_1 and PLAYER_BUTTON_2 to whatever you like.
 * The ball lights need to be in a sequential series of pins starting
 * with BALL_PIN_START and ending with BALL_PIN_END.  The code
 * will auto-adapt to however many pins you use, as long as you tell
 * it where the start and end points are.
 */

#define PLAYER_BUTTON_1 3
#define PLAYER_BUTTON_2 2

#define BALL_PIN_START 5
#define BALL_PIN_END 9

#define BALL_SPEED 1000
#define TURN_DELAY 1000

EvtManager mgr;

int currentPlayer;
int numBallPositions = BALL_PIN_END - BALL_PIN_START + 1;
int lastBallPosition = BALL_PIN_END - BALL_PIN_START;
int currentBallPosition;

void setup() {
  pinMode(PLAYER_BUTTON_1, INPUT);
  pinMode(PLAYER_BUTTON_2, INPUT);
  for(int i = BALL_PIN_START; i <= BALL_PIN_END; i++) {
    pinMode(i, OUTPUT);
  }

  currentPlayer = 1;
  readyPlayer();
}

void readyPlayer() {
  mgr.resetContext();

  if(currentPlayer == 1) {
    currentBallPosition = 0;
  } else {
    currentBallPosition = lastBallPosition;
  }
  showBall(currentBallPosition);

  mgr.addListener(new EvtPinListener(PLAYER_BUTTON_1, (EvtAction)startNextPlayer));
}

void showBall(int offset) {
  for(int i = 0; i + BALL_PIN_START <= BALL_PIN_END; i++) {
    if(i == offset) {
      digitalWrite(BALL_PIN_START + i, HIGH);
    } else {
      digitalWrite(BALL_PIN_START + i, LOW);
    }
  }
}

void showAll() {
  for(int i = 0; i + BALL_PIN_START <= BALL_PIN_END; i++) {
    digitalWrite(BALL_PIN_START + i, HIGH);
  }
}

bool startNextPlayer() {
  mgr.resetContext();

  mgr.addListener(new EvtTimeListener(BALL_SPEED, true, (EvtAction)moveBall));

  if(currentPlayer == 1) {
    currentPlayer = 2;
    mgr.addListener(new EvtPinListener(PLAYER_BUTTON_2, (EvtAction)swingRacket));
    currentBallPosition = 0;
  } else {
    currentPlayer = 1;
    mgr.addListener(new EvtPinListener(PLAYER_BUTTON_1, (EvtAction)swingRacket));
    currentBallPosition = lastBallPosition;
  }

  return true;
}


bool moveBall() {
  if(currentPlayer == 1) {
    currentBallPosition--;
    if(currentBallPosition < 0) {
      return missedBall();
    }
  } else {
    currentBallPosition++;
    if(currentBallPosition > lastBallPosition) {
      return missedBall();
    }
  }

  showBall(currentBallPosition);
  return false;
}


bool swingRacket() {
  if((currentPlayer == 1 && currentBallPosition == 0) || (currentPlayer == 2 && currentBallPosition == lastBallPosition)) {
    return hitBall();
  } else {
    return missedBall();
  }
}

bool hitBall() {
  // If we hit, it moves to be the next player's ball
  return startNextPlayer();
}

bool missedBall() {
  mgr.resetContext();
  showAll();
  currentPlayer = 1;
  mgr.addListener(new EvtTimeListener(TURN_DELAY, false, (EvtAction)readyPlayer));

  return true;
}

USE_EVENTUALLY_LOOP(mgr)
