#include <Arduino.h>

// === Pin Definitions ===
// Y Axis
const int stepPinY = 5; 
const int enPinY   = 6;
const int dirPinY  = 7; 
const int endstopY = A1;

// X1 Axis
const int stepPinX1 = 2; 
const int enPinX1   = 3;
const int dirPinX1  = 4; 
const int endstopX1 = A0;

// X2 Axis
const int stepPinX2 = 11; 
const int enPinX2   = 12;
const int dirPinX2  = 13; 
const int endstopX2 = A3;

// Z Axis
const int stepPinZ = 8;
const int enPinZ  = 9;
const int dirPinZ = 10;
const int endstopZ = A2;

const int endstopPen = A2;


// === Stepper Settings ===
int stepDelay = 25;   // microseconds between HIGH/LOW (slower for testing)
const int stepDelayWrite = 25;   // microseconds between HIGH/LOW (slower for testing)
const int stepDelayMove = 13;
const int stepDelayPen = 10;

bool calibrated = false;
bool writing = false;


// === Setup ===
void setup() {
  // Stepper pins
  pinMode(stepPinY, OUTPUT); 
  pinMode(dirPinY, OUTPUT);
  pinMode(enPinY, OUTPUT);

  pinMode(stepPinX1, OUTPUT); 
  pinMode(dirPinX1, OUTPUT);
  pinMode(enPinX1, OUTPUT);

  pinMode(stepPinX2, OUTPUT); 
  pinMode(dirPinX2, OUTPUT);
  pinMode(enPinX2, OUTPUT);

  pinMode(stepPinZ, OUTPUT); 
  pinMode(dirPinZ, OUTPUT);
  pinMode(enPinZ, OUTPUT);

  // Endstops
  pinMode(endstopY, INPUT);
  pinMode(endstopX1, INPUT);
  pinMode(endstopX2, INPUT);
  pinMode(endstopZ, INPUT);

  pinMode(endstopPen, INPUT);

  // Enable all drivers (LOW = enabled for most drivers like A4988/DRV8825)
  digitalWrite(enPinY, LOW);
  digitalWrite(enPinX1, LOW);
  digitalWrite(enPinX2, LOW);
  digitalWrite(enPinZ, LOW);

  // Start serial
  Serial.begin(2000000);
  Serial.println("Stepper test starting...");
}

void stepMotorZ(bool dir, long int steps) {
  digitalWrite(dirPinZ, !dir);

  for (long int i = 0; i < steps; i++) {
    digitalWrite(stepPinZ, HIGH);
    delayMicroseconds(stepDelayPen);
    digitalWrite(stepPinZ, LOW);
    delayMicroseconds(stepDelayPen);
  }
}

void calibrate() {
    if (calibrated) return;  // already done

    // Set motor directions
    digitalWrite(dirPinX1, LOW);
    digitalWrite(dirPinX2, HIGH);
    digitalWrite(dirPinY, LOW);
    digitalWrite(dirPinZ, HIGH);


    while (digitalRead(endstopPen) == LOW) {
        stepMotorZ(LOW, 100);
    }
    // move Z up a safe distance
    stepMotorZ(HIGH, 2000);

    while (digitalRead(endstopY) == LOW) {
      for (int i = 0; i < 100; i++) {
        digitalWrite(stepPinY, HIGH);
        delayMicroseconds(stepDelayMove);
        digitalWrite(stepPinY, LOW);
        delayMicroseconds(stepDelayMove);
      }
    }

    while (digitalRead(endstopX1) == LOW || digitalRead(endstopX2) == LOW) {
      for (int i = 0; i < 100; i++) {
        digitalWrite(stepPinX1, HIGH);
        digitalWrite(stepPinX2, HIGH);
        delayMicroseconds(stepDelayMove);
        digitalWrite(stepPinX1, LOW);
        digitalWrite(stepPinX2, LOW);
        delayMicroseconds(stepDelayMove);
      }
    }

    calibrated = true;
    stepMotorXY(0, 30000);
}

void stepMotorXY(long stepsX, long stepsY) {
  if (stepsX > 0) {
    digitalWrite(dirPinX1, HIGH);
    digitalWrite(dirPinX2, LOW);
  } else {
    digitalWrite(dirPinX1, LOW);
    digitalWrite(dirPinX2, HIGH);
  }

  if (stepsY > 0) {
    digitalWrite(dirPinY, HIGH);
  } else {
    digitalWrite(dirPinY, LOW);
  }

  long dx = abs(stepsX);
  long dy = abs(stepsY);

  long sx = (stepsX > 0) ? 1 : -1;
  long sy = (stepsY > 0) ? 1 : -1;

  long err = dx - dy;

  while (dx > 0 || dy > 0) {
    long prevDx = dx;
    long prevDy = dy;

    // Step X
    if (err > -dy) {
      err -= dy;
      dx--;
      digitalWrite(stepPinX1, HIGH);
      digitalWrite(stepPinX2, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPinX1, LOW);
      digitalWrite(stepPinX2, LOW);
      delayMicroseconds(stepDelay);
    }

    // Step Y
    if (err < dx) {
      err += dx;
      dy--;
      digitalWrite(stepPinY, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPinY, LOW);
      delayMicroseconds(stepDelay);
    }

    // Check if nothing changed, stop loop
    if (dx == prevDx && dy == prevDy) {
      break;
    }
  }
}

void togglePen(bool write) {
    if (write) {
        // Lower pen until endstop is triggered
        while (HIGH) {
          if (digitalRead(endstopPen) == HIGH) {
            writing = true;
            break;
          }
            stepMotorZ(LOW, 100);
        }
    } else {
        // Raise pen only if it’s not already up
        while (digitalRead(endstopPen) == HIGH) {
            stepMotorZ(HIGH, 500);
        }
    }
}

void loop() {
  // --- Ensure motors are calibrated ---
  if (writing) {
    stepDelay = stepDelayWrite;
  } else {
    stepDelay = stepDelayMove;
  }
  if (!calibrated) {
    calibrate();
    Serial.println("Calibration done.");
  }

  // --- Read serial commands ---
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Pen commands
    if (input == "PEN_DOWN") {
      togglePen(true);
      // Serial.println("Pen down");
    } else if (input == "PEN_UP") {
      togglePen(false);
      // Serial.println("Pen up");
    } else {
      // XY move command
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        long x = input.substring(0, commaIndex).toInt();
        long y = input.substring(commaIndex + 1).toInt();
        stepMotorXY(x, y);
        // Serial.print("Moved to X:");
        // Serial.print(x);
        // Serial.print(" Y:");
        // Serial.println(y);
      }
    }
  }
}