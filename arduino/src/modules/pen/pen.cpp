#include <./modules/pen/pen.h>

Pen::Pen(Motor* motor, Endstop* endstop) {
    this->motor = motor;
    this->endstop = endstop;
}

void Pen::write(bool write, uint8_t step_delay) {
    if (write) {
        motor->change_direction(HIGH);
        while (!endstop->is_pressed()) {
            motor->step(step_delay);
        }
    } else {
        motor->change_direction(LOW);
        while (endstop->is_pressed()) {
            motor->step(step_delay);
        }
        motor->run(1000, step_delay);
    }
}