#include "./modules/motor/double_motor.h"
#include "./modules/motor/motor.h"

DoubleMotor::DoubleMotor(Motor* motor1, Motor* motor2) {
    this->motor1 = motor1;
    this->motor2 = motor2;
}

void DoubleMotor::setup() {
    motor1->setup();
    motor2->setup();
}

void DoubleMotor::change_direction(bool clockwise) {
    motor1->change_direction(clockwise);
    motor2->change_direction(!clockwise);
}

void DoubleMotor::run(uint16_t steps, uint8_t step_delay) {
    for (uint16_t i = 0; i < steps; i++) {
        digitalWrite(motor1->step_pin, HIGH);
        digitalWrite(motor2->step_pin, HIGH);
        delayMicroseconds(step_delay);
        digitalWrite(motor1->step_pin, LOW);
        digitalWrite(motor2->step_pin, LOW);
        delayMicroseconds(step_delay);
    }
}