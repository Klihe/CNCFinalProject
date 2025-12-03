#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

class Commands {
    public:
        String* command;

        Commands(String* command);

        void check();
};

#endif // SERIAL_H