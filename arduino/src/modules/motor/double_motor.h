#ifndef DOUBLE_MOTOR_H
#define DOUBLE_MOTOR_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/double_endstop.h>
#include <./modules/state/state.h>

class DoubleMotor {
    private:
        Motor* motor1_;
        Motor* motor2_;
        DoubleEndstop* double_endstop_;
        uint8_t* step_delay_;
        uint16_t* changing_var_;
        bool direction_ = LOW;

        void step_();

    public:
        DoubleMotor(
            Motor* motor1,
            Motor* motor2,
            DoubleEndstop* double_endstop,
            uint8_t* step_delay,
            uint16_t* changing_var
        );

        void setup();
        void change_direction(bool clockwise);
        void calibrate();
        void run(uint16_t steps);
};

#endif