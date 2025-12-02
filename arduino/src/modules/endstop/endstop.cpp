#include "./modules/endstop/endstop.h"

Endstop::Endstop(int pin) {
    this->pin = pin;
}

void Endstop::setup() {
    pinMode(pin, INPUT);
}

bool Endstop::is_pressed() {
    return digitalRead(pin);
}