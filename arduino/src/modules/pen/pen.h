#ifndef PEN_H
#define PEN_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/endstop.h>

class Pen {
    public:
        Motor* motor;
        Endstop* endstop;

    Pen(Motor* motor, Endstop* endstop);

    void write(bool write);
};

#endif // PEN_H