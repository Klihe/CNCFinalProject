#ifndef DOUBLE_MOTOR_H
#define DOUBLE_MOTOR_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/double_endstop.h>

class DoubleMotor {
    private:
        Motor* motor1_;
        Motor* motor2_;
        DoubleEndstop* double_endstop_;
        uint8_t* step_delay_;

    public:
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