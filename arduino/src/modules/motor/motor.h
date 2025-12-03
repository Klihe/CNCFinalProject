#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <./modules/endstop/endstop.h>

class Motor {
public:
    uint8_t step_pin;
    uint8_t enable_pin;
    uint8_t direction_pin;

    Motor(uint8_t step_pin, uint8_t enable_pin, uint8_t direction_pin);

    void setup();
    void calibrate();
    void change_direction(bool clockwise);
    void step(uint8_t step_delay);
    void run(uint16_t steps, uint8_t step_delay);
};

#endif