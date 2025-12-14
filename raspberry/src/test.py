from modules.machine import Machine
import time
from modules.font import Text

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(15)

    while True:
        machine.commands.next_page()
        time.sleep(5)
