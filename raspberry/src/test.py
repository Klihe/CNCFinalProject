from modules.machine import Machine
import time

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(8)

    machine.commands.next_line()
    text = "a" * 75
    machine.write_text(text)
    machine.commands.next_line()
