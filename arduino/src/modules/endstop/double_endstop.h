#ifndef DOUBLE_ENDSTOP_H
#define DOUBLE_ENDSTOP_H

#include <./modules/endstop/endstop.h>

class DoubleEndstop {
    public:
        Endstop* endstop1;
        Endstop* endstop2;

        DoubleEndstop(Endstop* endstop1, Endstop* endstop2);

        void setup();
        bool is_pressed();
};

#endif