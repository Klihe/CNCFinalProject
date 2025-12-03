class _Communication: # -> Const
    PORT: str = "/dev/ttyUSB0"
    BAUD_RATE: int = 2_000_000

class _Slicing: # -> Const
    DISTANCE_THRESHOLD: float = 1e-4 # seconds per unit
    STEP_MULTIPLIER: int = 100
    DELAY_PER_UNIT: float = 12e-5
    CHUNK_SIZE: int = 2_000

class _Font: # -> Const
    SCALE: float = 0.5
    SPACING: int = 0
    NAME: str = "futural"

class _Commands: # -> Const
    PEN_UP: str = "PEN_UP"
    PEN_DOWN: str = "PEN_DOWN"
    CHUNK: str = "{chunk_dx}, {chunk_dy}"
    NEW_PAGE: str = "{page}_PAGE"
    NEXT_LINE: str = "NEXT_LINE"

class Const:
    COMMUNICATION = _Communication()
    SLICING = _Slicing()
    FONT = _Font()
    COMMANDS = _Commands()
