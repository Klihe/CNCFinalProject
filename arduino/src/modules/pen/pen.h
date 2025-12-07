#ifndef PEN_H
#define PEN_H

#include <./modules/motor/motor.h>
#include <./modules/endstop/endstop.h>

class Pen {
    private:
        Motor* motor_;
        Endstop* endstop_;

    public:
        Pen(Motor* motor, Endstop* endstop);

        void write(bool write, uint8_t* step_delay);
};

#endif // PEN_H