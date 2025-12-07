#include "./modules/motor/motor.h"

Motor::Motor(
    uint8_t step_pin,
    uint8_t enable_pin,
    uint8_t direction_pin, 
    Endstop* endstop,
    uint8_t* step_delay
) {
    this->step_pin_ = step_pin;
    this->enable_pin_ = enable_pin;
    this->direction_pin_ = direction_pin;

    this->endstop_ = endstop;
    this->step_delay_ = step_delay;
}

void Motor::setup() {
    pinMode(step_pin_, OUTPUT);
    pinMode(enable_pin_, OUTPUT);
    pinMode(direction_pin_, OUTPUT);
}

void Motor::change_direction(bool clockwise) {
    digitalWrite(direction_pin_, clockwise);
}

void Motor::step() {
    digitalWrite(step_pin_, HIGH);
    delayMicroseconds(*step_delay_);
    digitalWrite(step_pin_, LOW);
    delayMicroseconds(*step_delay_);
}

void Motor::step_raw(bool level) {
    digitalWrite(step_pin_, level ? HIGH : LOW);
}

void Motor::calibrate() {
    while (!endstop_->is_pressed()) {
        step();
    }
}

void Motor::run(uint16_t steps) {
    for (uint32_t i = 0; i < steps; i++) {
        step();
    }
}