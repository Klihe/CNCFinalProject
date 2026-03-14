#include "./modules/endstop/double_endstop.h"
#include "./modules/endstop/endstop.h"

DoubleEndstop::DoubleEndstop(Endstop* endstop1, Endstop* endstop2) {
    this->endstop1 = endstop1;
    this->endstop2 = endstop2;
}

void DoubleEndstop::setup() {
    endstop1->setup();
    endstop2->setup();
}

bool DoubleEndstop::is_pressed() {
    if (endstop1->is_pressed() && endstop2->is_pressed()) {
        return HIGH;
    }

    return LOW;
}