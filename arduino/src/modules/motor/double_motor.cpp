#include "./modules/motor/double_motor.h"
#include "./modules/motor/motor.h"

DoubleMotor::DoubleMotor(
    Motor* motor1,
    Motor* motor2,
    DoubleEndstop* double_endstop,
    uint8_t* step_delay
) {
    this->motor1_ = motor1;
    this->motor2_ = motor2;

    this->double_endstop_ = double_endstop;
    this->step_delay_ = step_delay;
}

void DoubleMotor::setup() {
    motor1_->setup();
    motor2_->setup();
}

void DoubleMotor::change_direction(bool clockwise) {
    motor1_->change_direction(clockwise);
    motor2_->change_direction(!clockwise);
}

void DoubleMotor::step() {
    motor1_->step_raw(HIGH);
    motor2_->step_raw(HIGH);
    delayMicroseconds(*step_delay_);
    motor1_->step_raw(LOW);
    motor2_->step_raw(LOW);
    delayMicroseconds(*step_delay_);
}

void DoubleMotor::calibrate() {
    while (!double_endstop_->is_pressed()) {
        step();
    }
}

void DoubleMotor::run(uint16_t steps) {
    for (uint32_t i = 0; i < steps; i++) {
        step();
    }
}