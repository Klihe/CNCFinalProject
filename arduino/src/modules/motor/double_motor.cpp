#include "./modules/motor/double_motor.h"

DoubleMotor::DoubleMotor(
    Motor* motor1,
    Motor* motor2,
    DoubleEndstop* double_endstop,
    uint8_t* step_delay,
    uint16_t* changing_var
) {
    this->motor1_ = motor1;
    this->motor2_ = motor2;

    this->double_endstop_ = double_endstop;
    this->step_delay_ = step_delay;
    this->changing_var_ = changing_var;
}

void DoubleMotor::setup() {
    motor1_->setup();
    motor2_->setup();
}

void DoubleMotor::change_direction(bool clockwise) {
    direction_ = clockwise ? HIGH : LOW;
    motor1_->change_direction(direction_);
    motor2_->change_direction(!direction_);
}

void DoubleMotor::step_() {
    motor1_->step_raw(HIGH);
    motor2_->step_raw(HIGH);
    delayMicroseconds(*step_delay_);
    motor1_->step_raw(LOW);
    motor2_->step_raw(LOW);
    delayMicroseconds(*step_delay_);
}

void DoubleMotor::calibrate() {
    while (!double_endstop_->is_pressed()) {
        step_();
    }
    *changing_var_ = 0;
}

void DoubleMotor::run(uint16_t steps) {
    for (uint16_t i = 0; i < steps; i++) {
        step_();
    }
    
    if (direction_) {
        *changing_var_ += (steps + 1);
    } else {
        *changing_var_ -= (steps + 1);
    }
}