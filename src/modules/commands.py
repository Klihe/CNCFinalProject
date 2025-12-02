import time

from serial import Serial

from const import Const


class Commands:
    PORT: str = Const.COMMUNICATION.PORT
    BAUD_RATE: int = Const.COMMUNICATION.BAUD_RATE

    DELAY_PER_UNIT: float = Const.SLICING.DELAY_PER_UNIT

    PEN_UP: str = Const.COMMANDS.PEN_UP
    PEN_DOWN: str = Const.COMMANDS.PEN_DOWN
    CHUNK: str = Const.COMMANDS.CHUNK

    def __init__(self) -> None:
        self.serial_communication = Serial(self.PORT, self.BAUD_RATE, timeout=1)

    def _base_command(self, command: str, distance: int) -> None:
        self.serial_communication.write(command.encode())
        self.serial_communication.flush()
        time.sleep(distance * self.DELAY_PER_UNIT)

    def pen_up(self) -> None:
        self._base_command(self.PEN_UP, 10)

    def pen_down(self) -> None:
        self._base_command(self.PEN_DOWN, 10)

    def send_chunk(self, chunk_dx: int, chunk_dy: int) -> None:
        distance = (chunk_dx**2 + chunk_dy**2)**0.5

        self._base_command(
            self.CHUNK.format(chunk_dx, chunk_dy),
            distance,
        )
