#include "./modules/endstop/endstop.h"

Endstop::Endstop(uint8_t pin) {
    this->pin_ = pin;
}

void Endstop::setup() {
    pinMode(pin_, INPUT);
}

bool Endstop::is_pressed() {
    return digitalRead(pin_);
}