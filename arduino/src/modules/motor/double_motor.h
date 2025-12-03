#ifndef DOUBLE_MOTOR_H
#define DOUBLE_MOTOR_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/double_endstop.h>

class DoubleMotor {
    public:
        Motor* motor1;
        Motor* motor2;
        DoubleEndstop* double_endstop;
        uint8_t* step_delay;

        DoubleMotor(
            Motor* motor1,
            Motor* motor2,
            DoubleEndstop* double_endstop,
            uint8_t* step_delay
        );

        void setup();
        void change_direction(bool clockwise);
        void step();
        void calibrate();
        void run(uint16_t steps);
};

#endif