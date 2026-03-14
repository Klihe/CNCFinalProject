from loguru import logger

from const.const import Const
from modules.commands import Commands
from modules.font import Text
from modules.helpers import distance


class Machine:
    def __init__(self, port: None | str = None) -> None:
        logger.info("Initializing machine")
        self.commands = Commands(port)
        self.text = Text()
        logger.success("Machine ready")

    def write_text(self, text: str) -> None:
        logger.info(f"Writing text ({len(text)} chars)")
        lines, start_char_strokes, end_char_strokes = self.text.text_to_strokes(text)
        logger.info(f"Text split into {len(lines)} lines")

        for line_idx, line in enumerate(lines):
            if line_idx >= 60:
                logger.warning("Reached 60-line limit, stopping")
                break

            if line_idx > 0 and line_idx % 30 == 0:
                logger.info(f"Page break at line {line_idx}")
                self.commands.next_page()

            logger.debug(f"Writing line {line_idx + 1}/{len(lines)}: {line.text!r}")
            self._write_line(line, start_char_strokes, end_char_strokes)

    def _write_line(self, line, start_char_strokes: int, end_char_strokes: int) -> None:
        # Compute scaled strokes for the entire line text
        raw_lines = line.font.lines_for_text(line.text)
        strokes = []
        for raw in raw_lines:
            if raw:
                scaled = [(y * Const.FONT.SCALE, -x * Const.FONT.SCALE) for x, y in raw]
                strokes.append(scaled)

        # Draw all strokes
        self.commands.pen_up()
        current_x, current_y = 0, 0
        pen_is_down = False
        last_pen_action_pos = (0, 0)

        for stroke_idx, stroke in enumerate(strokes):
            if len(stroke) < 1:
                continue

            # Determine if this stroke is a calibration character (start or end)
            is_calibration = (stroke_idx < start_char_strokes or stroke_idx >= len(strokes) - end_char_strokes)

            x0, y0 = stroke[0]
            dist_to_last = distance(last_pen_action_pos, (x0, y0))

            # PEN UP if needed for long travel
            if pen_is_down and dist_to_last > Const.SLICING.DISTANCE_THRESHOLD:
                self.commands.pen_up()
                pen_is_down = False

            # Move to start of stroke
            dx = int((x0 - current_x) * Const.SLICING.STEP_MULTIPLIER)
            dy = int((y0 - current_y) * Const.SLICING.STEP_MULTIPLIER)
            self.commands.move(dx, dy)
            current_x, current_y = x0, y0

            # PEN DOWN to draw (skip for calibration characters)
            if not is_calibration and not pen_is_down:
                self.commands.pen_down()
                pen_is_down = True
            elif is_calibration and pen_is_down:
                self.commands.pen_up()
                pen_is_down = False

            last_pen_action_pos = (current_x, current_y)

            # Draw the stroke
            for x, y in stroke[1:]:
                dx = int((x - current_x) * Const.SLICING.STEP_MULTIPLIER)
                dy = int((y - current_y) * Const.SLICING.STEP_MULTIPLIER)
                self.commands.move(dx, dy)
                current_x, current_y = x, y
                last_pen_action_pos = (current_x, current_y)

        # PEN UP at the end of line
        if pen_is_down:
            self.commands.pen_up()
            pen_is_down = False

        # Advance to next line
        self.commands.next_line()

    def calibrate(self) -> None:
        ...

    def change_page(self) -> None:
        ...
