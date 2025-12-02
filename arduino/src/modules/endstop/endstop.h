#ifndef ENDSTOP_H
#define ENDSTOP_H

#include <Arduino.h>

class Endstop {
    public:
        int pin;

    Endstop(int pin);

    void setup();
    bool is_pressed();
};

#endif