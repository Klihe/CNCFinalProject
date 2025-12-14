from typing import Literal

from const.const import Const
from serial import Serial


class Commands:
    PORT: str = Const.COMMUNICATION.PORT
    BAUD_RATE: int = Const.COMMUNICATION.BAUD_RATE

    CHUNK_SIZE: float = Const.SLICING.CHUNK_SIZE

    PEN_UP: str = Const.COMMANDS.PEN_UP
    PEN_DOWN: str = Const.COMMANDS.PEN_DOWN
    CHUNK: str = Const.COMMANDS.CHUNK
    NEW_PAGE: str = Const.COMMANDS.NEW_PAGE
    NEXT_LINE: str = Const.COMMANDS.NEXT_LINE

    def __init__(self, port: None | str = None) -> None:
        self.serial_communication = Serial(port or self.PORT, self.BAUD_RATE, timeout=1)

    def _base_command(self, command: str) -> None:
        self.serial_communication.write((command+"\n").encode())
        self.serial_communication.flush()

        while True:
            line = self.serial_communication.readline().decode().strip()
            if line == "DONE":
                break

    def pen_up(self) -> None:
        self._base_command(self.PEN_UP)

    def pen_down(self) -> None:
        self._base_command(self.PEN_DOWN)

    def move(self, dx: float, dy: float) -> None:
        while abs(dx) > 0 or abs(dy) > 0:
            chunk_dx = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dx))
            chunk_dy = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dy))

            command = self.CHUNK.format(chunk_dx=chunk_dx, chunk_dy=chunk_dy)
            self._base_command(command)

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
        self._base_command(self.NEXT_LINE)
