#include "./modules/endstop/endstop.h"

Endstop::Endstop(uint8_t pin) {
    this->pin = pin;
}

void Endstop::setup() {
    pinMode(pin, INPUT);
}

bool Endstop::is_pressed() {
    return digitalRead(pin);
}