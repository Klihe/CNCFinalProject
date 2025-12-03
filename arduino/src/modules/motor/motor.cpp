#include "./modules/motor/motor.h"

Motor::Motor(
    uint8_t step_pin,
    uint8_t enable_pin,
    uint8_t direction_pin, 
    Endstop* endstop
) {
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    this->direction_pin = direction_pin;

    this->endstop = endstop;
}

void Motor::setup() {
    pinMode(step_pin, OUTPUT);
    pinMode(enable_pin, OUTPUT);
    pinMode(direction_pin, OUTPUT);
}

void Motor::change_direction(bool clockwise) {
    digitalWrite(direction_pin, clockwise);
}

void Motor::step(uint8_t step_delay) {
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(step_delay);
    digitalWrite(step_pin, LOW);
    delayMicroseconds(step_delay);
}

void Motor::calibrate(uint8_t step_delay) {
    while (!endstop->is_pressed()) {
        step(step_delay);
    }
}

void Motor::run(uint16_t steps, uint8_t step_delay) {
    for (uint16_t i = 0; i < steps; i++) {
        step(step_delay);
    }
}