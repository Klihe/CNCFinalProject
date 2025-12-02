import time
from math import sqrt
from typing import Literal

from serial import Serial

from const.const import Const


class Commands:
    PORT: str = Const.COMMUNICATION.PORT
    BAUD_RATE: int = Const.COMMUNICATION.BAUD_RATE

    DELAY_PER_UNIT: float = Const.SLICING.DELAY_PER_UNIT
    CHUNK_SIZE: float = Const.SLICING.CHUNK_SIZE

    PEN_UP: str = Const.COMMANDS.PEN_UP
    PEN_DOWN: str = Const.COMMANDS.PEN_DOWN
    CHUNK: str = Const.COMMANDS.CHUNK
    NEW_PAGE: str = Const.COMMANDS.NEW_PAGE
    NEXT_LINE: str = Const.COMMANDS.NEXT_LINE

    def __init__(self, port: None | str = None) -> None:
        self.serial_communication = Serial(port or self.PORT, self.BAUD_RATE, timeout=1)

    def _base_command(self, command: str, distance: float) -> None:
        self.serial_communication.write(command.encode())
        self.serial_communication.flush()
        time.sleep(distance * self.DELAY_PER_UNIT)

    def pen_up(self) -> None:
        # distance = ???
        self._base_command(self.PEN_UP, 10)

    def pen_down(self) -> None:
        # distance = ???
        self._base_command(self.PEN_DOWN, 10)

    def move(self, dx: float, dy: float) -> None:
        while abs(dx) > 0 or abs(dy) > 0:
            chunk_dx = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dx))
            chunk_dy = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dy))

            distance = sqrt(chunk_dx**2 + chunk_dy**2)

            command = self.CHUNK.format(chunk_dx=chunk_dx, chunk_dy=chunk_dy)
            self._base_command(command, distance)

            dx -= chunk_dx
            dy -= chunk_dy

    def new_page(self, page: Literal["LEFT", "RIGHT"]) -> None:
        # distance = ???
        # self._base_command(
        #     self.NEW_PAGE.format(page),
        #     distance
        # )
        ...

    def next_line(self) -> None:
        # distance = ???
        self._base_command(self.NEXT_LINE, 10)
