from modules.machine import Machine
import time

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(10)
    for _ in range(10):
        machine.write_text("test test test test test test test test test test test test test test test")
        machine.commands.next_line()
