#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    int stepPin;
    int enPin;
    int dirPin;

    Motor(int step, int en, int dir);

    void setup();
    void change_direction(bool clockwise);
    void run(long int steps, int step_delay);
};

#endif