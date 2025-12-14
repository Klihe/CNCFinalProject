#include <Arduino.h>
#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>
#include <./modules/endstop/endstop.h>
#include <./modules/endstop/double_endstop.h>
#include <./modules/move/move.h>
#include <./modules/pen/pen.h>
#include <./const/const.h>
#include <./modules/state/state.h>

State state{};

uint8_t* step_delay = &Const::STEP_DELAY_MOVING;

bool calibrated = false;
bool writing = false;

// Command queue
#define QUEUE_SIZE 20
String commandQueue[QUEUE_SIZE];
int queueHead = 0;  // Next position to write
int queueTail = 0;  // Next position to read
int queueCount = 0; // Number of items in queue

Endstop endstop_y1(A0);
Endstop endstop_y2(A3);
Endstop endstop_x(A1);
Endstop endstop_z(A2);

Endstop endstop_pen(A2);

DoubleEndstop double_endstop_y(&endstop_y1, &endstop_y2);

Motor motor_y1(2, 3, 4, &endstop_y1, state.step_delay, &state.y);
Motor motor_y2(11, 12, 13, &endstop_y2, state.step_delay, &state.y);
Motor motor_x(5, 6, 7, &endstop_x, state.step_delay, &state.x);
Motor motor_z(8, 9, 10, &endstop_z, state.step_delay, &state.z);

DoubleMotor double_motor_y(&motor_y1, &motor_y2, &double_endstop_y, state.step_delay, &state.y);

Move move(&motor_x, &double_motor_y);
Pen pen(&motor_z, &endstop_z);

void setup() {
  Serial.begin(Const::BAUDRATE);
  Serial.setTimeout(10);

  Serial.println("Initializing motors...");
  double_motor_y.setup();
  motor_x.setup();
  motor_z.setup();

  Serial.println("Initializing endstops...");
  double_endstop_y.setup();
  endstop_x.setup();
  endstop_z.setup();
  endstop_pen.setup();

  Serial.println("Setup complete...");
}

void calibrate() {
    Serial.println("Calibrating axis z...");
    motor_z.change_direction(HIGH);
    motor_z.calibrate();

    motor_z.change_direction(LOW);
    motor_z.run(5000);

    Serial.println("Calibrating axis y...");
    motor_x.change_direction(LOW);
    motor_x.calibrate();

    Serial.println("Calibrating axis x...");
    double_motor_y.change_direction(LOW);
    double_motor_y.calibrate();

    Serial.println("Moving to left-upper conrner");
    move.run(Const::MAX_X, Const::FIRST_LINE);
    Serial.print("After calibration - state.y: ");
    Serial.print(state.y);
    Serial.print(", state.x: ");
    Serial.println(state.x);

    state.currentLine = 0;
    calibrated = true;
}

void next_page() {
    Serial.println("Calibrating axis z...");
    motor_z.change_direction(HIGH);
    motor_z.calibrate();

    motor_z.change_direction(LOW);
    motor_z.run(5000);

    Serial.println("Calibrating axis y...");
    motor_x.change_direction(LOW);
    motor_x.calibrate();

    Serial.println("Calibrating axis x...");
    double_motor_y.change_direction(LOW);
    double_motor_y.calibrate();

    Serial.println("Moving to left-upper conrner");
    move.run(Const::MAX_X, Const::FIRST_LINE);
    Serial.print("After calibration - state.y: ");
    Serial.print(state.y);
    Serial.print(", state.x: ");
    Serial.println(state.x);

    state.currentLine = 0;
    calibrated = true;
}

void executeCommand(String input) {
  // Pen commands
  if (input == "PEN_DOWN") {
    pen.write(HIGH, step_delay);
  } else if (input == "PEN_UP") {
    pen.write(LOW, step_delay);
  } else if (input == "NEXT_LINE") {
    Serial.print(state.currentLine);
    Serial.print(state.y);
    Serial.println(state.x);
    
    state.currentLine = state.currentLine + 1;
    
    long targetY = (long)Const::FIRST_LINE + (long)Const::ONE_LINE_WIDTH * (long)state.currentLine;
    long deltaY = targetY - (long)state.y;
    
    Serial.print(state.currentLine);
    Serial.print(targetY);
    Serial.println(deltaY);

    move.run(-state.x, deltaY);
    
    Serial.print(state.y);
    Serial.println(state.x);
  } else {
    int commaIndex = input.indexOf(',');
    if (commaIndex != -1) {
      long y = input.substring(0, commaIndex).toInt();
      long x = input.substring(commaIndex + 1).toInt();
      move.run(x, y);
    }
  }
}

void loop() {
  if (!calibrated) {
    calibrate();
  }
  
  static String inputBuffer = "";
  while (Serial.available() > 0 && queueCount < QUEUE_SIZE) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      String input = inputBuffer;
      input.trim();
      inputBuffer = "";
      
      if (input.length() == 0) continue;

      commandQueue[queueHead] = input;
      queueHead = (queueHead + 1) % QUEUE_SIZE;
      queueCount++;
      
      Serial.println("DONE");
    } else {
      inputBuffer += c;
      if (inputBuffer.length() > 1024) inputBuffer = "";
    }
  }
  
  if (queueCount > 0) {
    String cmd = commandQueue[queueTail];
    queueTail = (queueTail + 1) % QUEUE_SIZE;
    queueCount--;
    
    executeCommand(cmd);
  }
}