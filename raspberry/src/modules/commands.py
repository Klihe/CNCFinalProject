from const.const import Const
from loguru import logger
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
        resolved_port = port or self.PORT
        logger.info(f"Connecting to serial port {resolved_port} at {self.BAUD_RATE} baud")
        self.serial_communication = Serial(resolved_port, self.BAUD_RATE, timeout=1)
        logger.success(f"Connected to {resolved_port}")

    def _base_command(self, command: str) -> None:
        logger.debug(f"Sending command: {command!r}")
        self.serial_communication.write((command+"\n").encode())
        self.serial_communication.flush()

        while True:
            line = self.serial_communication.readline().decode().strip()
            if line == "DONE":
                break

    def pen_up(self) -> None:
        logger.debug("Pen up")
        self._base_command(self.PEN_UP)

    def pen_down(self) -> None:
        logger.debug("Pen down")
        self._base_command(self.PEN_DOWN)

    def move(self, dx: float, dy: float) -> None:
        logger.debug(f"Move dx={dx}, dy={dy}")
        while abs(dx) > 0 or abs(dy) > 0:
            chunk_dx = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dx))
            chunk_dy = max(-self.CHUNK_SIZE, min(self.CHUNK_SIZE, dy))

            command = self.CHUNK.format(chunk_dx=chunk_dx, chunk_dy=chunk_dy)
            self._base_command(command)

            dx -= chunk_dx
            dy -= chunk_dy

    def next_page(self) -> None:
        logger.info("Advancing to next page")
        self._base_command(self.NEW_PAGE)

    def next_line(self) -> None:
        logger.info("Advancing to next line")
        self._base_command(self.NEXT_LINE)
