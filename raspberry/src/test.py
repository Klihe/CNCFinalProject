from modules.machine import Machine
import time

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(8)
    for _ in range(10):
        machine.write_text("                     a")
        machine.commands.next_line()
