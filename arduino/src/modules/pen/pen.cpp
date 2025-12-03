#include <./modules/pen/pen.h>
#include <./const/const.h>

Pen::Pen(Motor* motor, Endstop* endstop) {
    this->motor = motor;
    this->endstop = endstop;
}

void Pen::write(bool write) {
    if (write) {
        motor->change_direction(HIGH);
        motor->step_delay = &Const::STEP_DELAY_WRITING;
        while (!endstop->is_pressed()) {
            motor->step();
        }
    } else {
        motor->change_direction(LOW);
        motor->step_delay = &Const::STEP_DELAY_MOVING;
        while (endstop->is_pressed()) {
            motor->step();
        }
        motor->run(1000);
    }
}