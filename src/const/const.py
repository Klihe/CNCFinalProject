class _Communication: # -> Const
    PORT: str = "/dev/ttyUSB0"
    BAUD_RATE: int = 2_000_000

class _Data: # -> Const
    CHUNK_SIZE: int = 2_000
    BASE_TIME_PER_UNIT: float = 1e-3 # minimum distance to record new pen action

class _Slicing: # -> Const
    DIST_THRESHOLD: float = 1e-4 # seconds per unit

class _Font: # -> Const
    SCALE: float = 0.5
    SPACING: int = 0
    NAME: str = "futural"
    STEP_MULTIPLIER: int = 100

class _Commands: # -> Const
    PEN_UP: bytes = b"PEN_UP\n"
    PEN_DOWN: bytes = b"PEN_DOWN\n"

class Const:
    COMMUNICATION: _Communication
    DATA: _Data
    SLICING: _Slicing
    FONT: _Font
    COMMAND: _Commands
