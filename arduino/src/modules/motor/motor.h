#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <./modules/endstop/endstop.h>

class Motor {
    private:
        uint8_t step_pin_;
        uint8_t enable_pin_;
        uint8_t direction_pin_;

        Endstop* endstop_;
        uint8_t* step_delay_;

    public:
        Motor(
            uint8_t step_pin,
            uint8_t enable_pin,
            uint8_t direction_pin,
            Endstop* endstop,
            uint8_t* step_delay
        );

        void setup();
        void change_direction(bool clockwise);
        void step();
        void step_raw(bool level);
        void calibrate();
        void run(uint16_t steps);
};

#endif