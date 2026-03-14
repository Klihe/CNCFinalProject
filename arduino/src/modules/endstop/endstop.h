#ifndef ENDSTOP_H
#define ENDSTOP_H

#include <Arduino.h>

class Endstop {
    private:
        uint8_t pin_;

    public:
        Endstop(uint8_t pin);

        void setup();
        bool is_pressed();
};

#endif