#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include <./const/const.h>

class State {
    public:
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t z = 0;

        uint8_t* step_delay = &Const::STEP_DELAY_MOVING;

        State();
};

#endif // STATE_H