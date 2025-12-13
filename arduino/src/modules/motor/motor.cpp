#include "./modules/motor/motor.h"

Motor::Motor(
    uint8_t step_pin,
    uint8_t enable_pin,
    uint8_t direction_pin, 
    Endstop* endstop,
    uint8_t* step_delay,
    uint16_t* changing_var
) {
    this->step_pin_ = step_pin;
    this->enable_pin_ = enable_pin;
    this->direction_pin_ = direction_pin;

    this->endstop_ = endstop;
    this->step_delay_ = step_delay;
    this->changing_var_ = changing_var;
}

void Motor::setup() {
    pinMode(step_pin_, OUTPUT);
    pinMode(enable_pin_, OUTPUT);
    pinMode(direction_pin_, OUTPUT);
}

void Motor::change_direction(bool clockwise) {
    direction_ = clockwise ? HIGH : LOW;
    digitalWrite(direction_pin_, clockwise);
}

void Motor::step_() {
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
        step_();
    }
}

void Motor::run(uint16_t steps) {
    for (uint16_t i = 0; i < steps; i++) {
        step_();
    }

    if (direction_) {
        *changing_var_ += steps;
    } else {
        *changing_var_ -= steps;
    }
}