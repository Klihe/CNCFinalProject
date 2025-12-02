#include <Arduino.h>
#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>
#include <./modules/endstop/endstop.h>
#include <./modules/endstop/double_endstop.h>
#include <./modules/move/xy_move.h>

Motor motor_x1(2, 3, 4);
Motor motor_x2(11, 12, 13);
Motor motor_y(5, 6, 7);
Motor motor_z(8, 9, 10);

DoubleMotor double_motor_x(&motor_x1, &motor_x2);

Endstop endstop_x1(A0);
Endstop endstop_x2(A3);
Endstop endstop_y(A1);
Endstop endstop_z(A2);
Endstop endstop_pen(A2);

DoubleEndstop double_endstop_x(&endstop_x1, &endstop_x2);

XYMove xy_move(&double_motor_x, &motor_y);

int stepDelay = 25;
const int stepDelayWrite = 25;
const int stepDelayMove = 13;
const int stepDelayPen = 10;
const long int baudrate = 2000000;

const long int max_x = 65000;
const int max_y = 40000;

bool calibrated = false;
bool writing = false;

void setup() {
  Serial.begin(baudrate);

  Serial.println("Initializing motors...");
  double_motor_x.setup();
  motor_y.setup();
  motor_z.setup();

  Serial.println("Initializing endstops...");
  double_endstop_x.setup();
  endstop_y.setup();
  endstop_z.setup();
  endstop_pen.setup();

  Serial.println("Setup complete...");
}

void calibrate() {
    if (calibrated) return;

    Serial.println("Calibrating axis z...");
    motor_z.change_direction(HIGH);
    while (!endstop_pen.is_pressed()) {
        motor_z.run(100, stepDelayPen);
    }

    motor_z.change_direction(LOW);
    motor_z.run(5000, stepDelayPen);

    Serial.println("Calibrating axis y...");
    motor_y.change_direction(LOW);
    while (!endstop_y.is_pressed()) {
      motor_y.run(100, stepDelayMove);
    }

    Serial.println("Calibrating axis x...");
    double_motor_x.change_direction(LOW);
    while (!double_endstop_x.is_pressed()) {
      double_motor_x.run(100, stepDelayMove);
    }

    Serial.println("Moving to left-upper conrner");
    xy_move.run(1000, max_x, stepDelayMove);
    calibrated = true;
}

void togglePen(bool write) {
    if (write) {
        // Lower pen until endstop is triggered
        while (HIGH) {
          if (endstop_pen.is_pressed()) {
            writing = true;
            break;
          } 
            motor_z.change_direction(HIGH);
            motor_z.run(100, stepDelayPen);
        }
    } else {
        // Raise pen only if it’s not already up
        motor_z.change_direction(LOW);
        while (endstop_pen.is_pressed()) {
            motor_z.run(500, stepDelayPen);
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
  }

  // --- Read serial commands ---
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Pen commands
    if (input == "PEN_DOWN") {
      togglePen(true);
    } else if (input == "PEN_UP") {
      togglePen(false);
    } else if (input == "NEXT_LINE") {
      double_motor_x.change_direction(HIGH);
      double_motor_x.run(1200, stepDelayMove);
    } else {
      // XY move command
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        long x = input.substring(0, commaIndex).toInt();
        long y = input.substring(commaIndex + 1).toInt();
        xy_move.run(x, y, stepDelayWrite);
      }
    }
  }
}