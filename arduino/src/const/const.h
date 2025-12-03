#ifndef CONST_H
#define CONST_H

#include <Arduino.h>

class Const {
    public:
    static uint32_t BAUDRATE;

    static uint16_t MAX_X;
    static uint16_t MAX_Y;
    static uint16_t ONE_LINE_WIDTH;

    static uint8_t STEP_DELAY_WRITING;
    static uint8_t STEP_DELAY_MOVING;


};

#endif // CONST_H