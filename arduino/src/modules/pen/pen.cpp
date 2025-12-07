#include <./modules/pen/pen.h>
#include <./const/const.h>

Pen::Pen(Motor* motor, Endstop* endstop) {
    this->motor_ = motor;
    this->endstop_ = endstop;
}

void Pen::write(bool write, uint8_t* step_delay) {
    if (write) {
        motor_->change_direction(HIGH);
        step_delay = &Const::STEP_DELAY_MOVING;
        while (!endstop_->is_pressed()) {
            motor_->run(1);
        }
    } else {
        motor_->change_direction(LOW);
        step_delay = &Const::STEP_DELAY_MOVING;
        while (endstop_->is_pressed()) {
            motor_->run(1);
        }
        motor_->run(1000);
    }
}