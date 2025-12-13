from const.const import Const
from modules.commands import Commands
from modules.font import Font
from modules.helpers import distance


class Machine:
    def __init__(self, port: None | str = None) -> None:
        self.commands = Commands(port)
        self.font = Font()

    def write_text(self, text: str) -> None:
        # --- Draw all strokes ---
        self.commands.pen_up()
        current_x, current_y = 0, 0
        pen_is_down = False
        last_pen_action_pos = (0, 0)

        all_strokes, first_pipe_count, text_stroke_count = self.font.text_to_strokes(text)

        # Calculate indices for text section
        text_start_idx = first_pipe_count
        text_end_idx = text_start_idx + text_stroke_count

        for i, stroke in enumerate(all_strokes):
            if len(stroke) < 1:
                continue

            x0, y0 = stroke[0]
            dist_to_last = distance(last_pen_action_pos, (x0, y0))

            # Determine if this stroke is part of the actual text (not pipes)
            is_text_stroke = text_start_idx <= i < text_end_idx

            # --- PEN UP if needed ---
            if pen_is_down and dist_to_last > Const.SLICING.DISTANCE_THRESHOLD:
                self.commands.pen_up()
                pen_is_down = False

            # --- Move to start of stroke ---
            dx = int((x0 - current_x) * Const.SLICING.STEP_MULTIPLIER)
            dy = int((y0 - current_y) * Const.SLICING.STEP_MULTIPLIER)
            self.commands.move(dx, dy)
            current_x, current_y = x0, y0

            # --- PEN DOWN only for text strokes (not pipes) ---
            if is_text_stroke and not pen_is_down:
                self.commands.pen_down()
                pen_is_down = True
            elif not is_text_stroke and pen_is_down:
                self.commands.pen_up()
                pen_is_down = False

            last_pen_action_pos = (current_x, current_y)

            # --- Draw the stroke ---
            for x, y in stroke[1:]:
                dx = int((x - current_x) * Const.SLICING.STEP_MULTIPLIER)
                dy = int((y - current_y) * Const.SLICING.STEP_MULTIPLIER)
                self.commands.move(dx, dy)
                current_x, current_y = x, y
                last_pen_action_pos = (current_x, current_y)


        # --- PEN UP at the end ---
        if pen_is_down:
            self.commands.pen_up()
            pen_is_down = False

    def calibrate(self) -> None:
        ...

    def change_page(self) -> None:
        ...
