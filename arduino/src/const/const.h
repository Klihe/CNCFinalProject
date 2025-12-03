#ifndef CONST_H
#define CONST_H

#include <Arduino.h>

class Const {
    public:
        static uint8_t STEP_DELAY_WRITING;
        static uint8_t STEP_DELAY_MOVING;

        static uint16_t MAX_X;
        static uint16_t MAX_Y;

        static uint32_t BAUDRATE;
};

#endif // CONST_H