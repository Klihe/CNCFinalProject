#include <./modules/commands/commands.h>

Commands::Commands(String* command) {
  this->command = command;
}

void Commands::check() {
  if (Serial.available() > 0) {
    *command = Serial.readStringUntil('\n');
    command->trim();
  }
}