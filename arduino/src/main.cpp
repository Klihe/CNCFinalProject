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
  double_motor_x.setup();
  motor_y.setup();
  motor_z.setup();


  double_endstop_x.setup();
  endstop_y.setup();
  endstop_z.setup();
  endstop_pen.setup();

  // Start serial
  Serial.begin(2000000);
}

void calibrate() {
    if (calibrated) return;  // already done

    motor_z.change_direction(HIGH);
    while (!endstop_pen.is_pressed()) {
        motor_z.run(100, stepDelayPen);
    }

    // move Z up a safe distance
    motor_z.change_direction(LOW);
    motor_z.run(2000, stepDelayPen);

    motor_y.change_direction(LOW);
    while (!endstop_y.is_pressed()) {
      motor_y.run(100, stepDelayMove);
    }

    double_motor_x.change_direction(LOW);
    while (!double_endstop_x.is_pressed()) {
      double_motor_x.run(100, stepDelayMove);
    }

    calibrated = true;
    xy_move.run(30000, 0, stepDelayMove);
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
    Serial.println("Calibration done.");
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