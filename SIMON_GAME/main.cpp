#include <Arduino.h>

const int buttons[4] = {2, 3, 4, 5};
const int leds[4] = {7, 8, 9, 10};
const int buzzer = 11;

const int notes[4] = {
  262, // C4
  330, // E4
  392, // G4
  523  // A4
};

int sequence[50];
int level = 1;

void startupAnimation();
void showSequence();
bool playerTurn();
int waitForButton();
void flashLED(int index);
void gameOver();

void setup() {

  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A0));

  sequence[0] = random(0, 4);

  startupAnimation();

  Serial.println("=================");
  Serial.println("   SIMON SAYS");
  Serial.println("=================");
  Serial.println("LEVEL 1");
}

void loop() {

  showSequence();

  bool success = playerTurn();

  if (success) {

    Serial.println("CORRECT!");

    // Victory sound
    

    tone(buzzer, 784, 200);
    delay(550);

    noTone(buzzer);

    level++;

    if (level > 50) {
      Serial.println("YOU WIN!");
      while (true);
    }

    sequence[level - 1] = random(0, 4);

    Serial.print("NEXT LEVEL: ");
    Serial.println(level);

    delay(2000);

  } else {

    Serial.println("WRONG!");
    Serial.println("GAME OVER!");

    gameOver();

    level = 1;
    sequence[0] = random(0, 4);

    Serial.println("RESTARTING...");
    delay(1000);

    Serial.println("LEVEL 1");
  }
}

void showSequence() {

  Serial.println();
  Serial.print("LEVEL ");
  Serial.println(level);

  delay(800);

  for (int i = 0; i < level; i++) {

    int current = sequence[i];

    digitalWrite(leds[current], HIGH);

    tone(buzzer, notes[current]);

    delay(450);

    digitalWrite(leds[current], LOW);

    noTone(buzzer);

    delay(250);
  }
}

bool playerTurn() {

  Serial.println("YOUR TURN");

  for (int i = 0; i < level; i++) {

    int pressed = waitForButton();

    flashLED(pressed);

    if (pressed != sequence[i]) {
      return false;
    }
  }

  return true;
}

int waitForButton() {

  while (true) {

    for (int i = 0; i < 4; i++) {

      if (digitalRead(buttons[i]) == LOW) {

        delay(20);

        while (digitalRead(buttons[i]) == LOW);

        return i;
      }
    }
  }
}

void flashLED(int index) {

  digitalWrite(leds[index], HIGH);

  tone(buzzer, notes[index]);

  delay(200);

  digitalWrite(leds[index], LOW);

  noTone(buzzer);

  delay(50);
}

void gameOver() {

  int failNotes[] = {
    523,
    440,
    392,
    262
  };

  for (int i = 0; i < 4; i++) {

    for (int j = 0; j < 4; j++) {
      digitalWrite(leds[j], HIGH);
    }

    tone(buzzer, failNotes[i]);

    delay(250);

    for (int j = 0; j < 4; j++) {
      digitalWrite(leds[j], LOW);
    }

    delay(100);
  }

  noTone(buzzer);
}

void startupAnimation() {

  int startNotes[] = {
    262,
    330,
    392,
    523
  };

  for (int i = 0; i < 4; i++) {

    digitalWrite(leds[i], HIGH);

    tone(buzzer, startNotes[i]);

    delay(180);

    digitalWrite(leds[i], LOW);

    delay(50);
  }

  noTone(buzzer);
}