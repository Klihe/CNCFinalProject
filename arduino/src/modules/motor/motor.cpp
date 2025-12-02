#include "./modules/motor/motor.h"

Motor::Motor(int stepPin, int enPin, int dirPin) {
    stepPin = stepPin;
    enPin = enPin;
    dirPin = dirPin;
}

void Motor::setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void Motor::change_direction(bool clockwise) {
    digitalWrite(dirPin, clockwise);
}

void Motor::run(long int steps, int step_delay) {
    for (long int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(step_delay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(step_delay);
    }
}