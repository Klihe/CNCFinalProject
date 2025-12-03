#ifndef DOUBLE_MOTOR_H
#define DOUBLE_MOTOR_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/double_endstop.h>

class DoubleMotor {
    public:
        Motor* motor1;
        Motor* motor2;
        DoubleEndstop* double_endstop;

        DoubleMotor(
            Motor* motor1,
            Motor* motor2,
            DoubleEndstop* double_endstop
        );

        void setup();
        void change_direction(bool clockwise);
        void step(uint8_t step_delay);
        void calibrate(uint8_t step_delay);
        void run(uint16_t steps, uint8_t step_delay);
};

#endif