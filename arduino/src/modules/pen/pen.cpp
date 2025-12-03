#include <./modules/pen/pen.h>

Pen::Pen(Motor* motor, Endstop* endstop) {
    this->motor = motor;
    this->endstop = endstop;
}

void Pen::write(bool write) {
    if (write) {
        motor->change_direction(HIGH);
        while (!endstop->is_pressed()) {
            motor->step();
        }
    } else {
        motor->change_direction(LOW);
        while (endstop->is_pressed()) {
            motor->step();
        }
        motor->run(1000);
    }
}