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

    static String COMMAND_PEN_DOWN;
    static String COMMAND_PEN_UP;
    static String COMMAND_NEXT_LINE;
};

#endif // CONST_H