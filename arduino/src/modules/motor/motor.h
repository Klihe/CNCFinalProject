#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <./modules/endstop/endstop.h>

class Motor {
public:
    uint8_t step_pin;
    uint8_t enable_pin;
    uint8_t direction_pin;

    Endstop* endstop;

    Motor(
        uint8_t step_pin,
        uint8_t enable_pin,
        uint8_t direction_pin,
        Endstop* endstop
    );

    void setup();
    void change_direction(bool clockwise);
    void step(uint8_t step_delay);
    void calibrate(uint8_t);
    void run(uint16_t steps, uint8_t step_delay);
};

#endif