#ifndef ENDSTOP_H
#define ENDSTOP_H

#include <Arduino.h>

class Endstop {
    public:
        uint8_t pin;

    Endstop(uint8_t pin);

    void setup();
    bool is_pressed();
};

#endif