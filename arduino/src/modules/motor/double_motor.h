#ifndef DOUBLE_MOTOR_H
#define DOUBLE_MOTOR_H

#include <./modules/motor/motor.h>

class DoubleMotor {
    public:
        Motor* motor1;
        Motor* motor2;

        DoubleMotor(Motor* motor1, Motor* motor2);

        void setup();
        void change_direction(bool clockwise);
        void run(long int steps, int step_delay);
};

#endif