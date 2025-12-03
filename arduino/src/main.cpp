#include <Arduino.h>
#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>
#include <./modules/endstop/endstop.h>
#include <./modules/endstop/double_endstop.h>
#include <./modules/move/move.h>
#include <./modules/pen/pen.h>

const uint8_t step_delay_write = 25;
const uint8_t step_delay_move = 13;
uint8_t step_delay = step_delay_move;

const uint16_t max_x = 65000;
const uint16_t max_y = 40000;

const uint32_t baudrate = 2000000;

bool calibrated = false;
bool writing = false;

Endstop endstop_x1(A0);
Endstop endstop_x2(A3);
Endstop endstop_y(A1);
Endstop endstop_z(A2);

Endstop endstop_pen(A2);

DoubleEndstop double_endstop_x(&endstop_x1, &endstop_x2);

Motor motor_x1(2, 3, 4, &endstop_x1, &step_delay);
Motor motor_x2(11, 12, 13, &endstop_x2, &step_delay);
Motor motor_y(5, 6, 7, &endstop_y, &step_delay);
Motor motor_z(8, 9, 10, &endstop_z, &step_delay);

DoubleMotor double_motor_x(&motor_x1, &motor_x2, &double_endstop_x, &step_delay);

Move move(&double_motor_x, &motor_y);
Pen pen(&motor_z, &endstop_z);

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
    motor_z.calibrate();

    motor_z.change_direction(LOW);
    motor_z.run(5000);

    Serial.println("Calibrating axis y...");
    motor_y.change_direction(LOW);
    motor_y.calibrate();

    Serial.println("Calibrating axis x...");
    double_motor_x.change_direction(LOW);
    double_motor_x.calibrate();

    Serial.println("Moving to left-upper conrner");
    move.run(1000, max_x);
    calibrated = true;
}

void loop() {
  // --- Ensure motors are calibrated ---
  if (writing) {
    step_delay = step_delay_move;
  } else {
    step_delay = step_delay_move;
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
      pen.write(HIGH);
    } else if (input == "PEN_UP") {
      pen.write(LOW);
    } else if (input == "NEXT_LINE") {
      double_motor_x.change_direction(HIGH);
      double_motor_x.run(1200);
    } else {
      // XY move command
      int commaIndex = input.indexOf(',');
      if (commaIndex != -1) {
        long x = input.substring(0, commaIndex).toInt();
        long y = input.substring(commaIndex + 1).toInt();
        move.run(x, y);
      }
    }
  }
}