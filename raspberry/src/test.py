from modules.machine import Machine
import time
from modules.font import Text

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(8)

    machine.write_text("i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i i")
    machine.commands.next_line()

    # font = Font()
    # font.text_to_strokes("ahoj ahoj ahoj")
