import re

from HersheyFonts import HersheyFonts
from loguru import logger
from const.const import Const

class Font(HersheyFonts):
    def __init__(self) -> None:
        super().__init__()
        logger.info(f"Loading font: {Const.FONT.NAME!r}")
        self.load_default_font(Const.FONT.NAME)
        logger.success(f"Font {Const.FONT.NAME!r} loaded")

class Line:
    def __init__(self, font: Font):
        self.font = font
        self.text = ""

    def add_word(self, word: str) -> bool:
        text_width = self._text_width(self.text + word)
        if text_width > Const.SLICING.LINE_WIDTH:
            logger.debug(f"Word {word!r} does not fit (width={text_width:.1f} > {Const.SLICING.LINE_WIDTH}), starting new line")
            return False

        self.text += word
        logger.debug(f"Added word {word!r}, line width now {text_width:.1f}/{Const.SLICING.LINE_WIDTH}")
        return True

    def end_line(self, start_char, end_char) -> None:
        logger.debug(f"Closing line: {self.text!r}")
        self.text = start_char + self.text + end_char

    def _text_width(self, text) -> float:
        if not text:
            return 0.0

        lines = self.font.lines_for_text(text)

        max_x = float("-inf")
        min_x = float("inf")

        strokes = []

        for line in lines:
            scaled_line = [(y * Const.FONT.SCALE, -x * Const.FONT.SCALE) for x, y in line]
            strokes.append(scaled_line)

        for stroke in strokes:
            for _, x in stroke:
                max_x = max(max_x, x)
                min_x = min(min_x, x)

        return (max_x - min_x) if max_x != float("-inf") else 0.0

class Text:
    def __init__(self) -> None:
        logger.info("Initializing Text renderer")
        self.font = Font()

    def text_to_strokes(self, text: str) -> tuple[list[Line], int, int]:
        logger.info(f"Converting text to strokes ({len(text)} chars)")
        start_char = "|"
        end_char = "j"

        start_char_number_of_strokes = len(list(self.font.lines_for_text(start_char)))
        end_char_number_of_strokes = len(list(self.font.lines_for_text(end_char)))
        logger.debug(f"Calibration strokes: start={start_char_number_of_strokes}, end={end_char_number_of_strokes}")

        words: list[str] = re.findall(r"\s+|\S+", text)
        logger.debug(f"Found {len(words)} tokens (words + whitespace)")

        lines = [Line(self.font)]
        index = 0

        for word in words:
            if lines[index].add_word(word):
                continue
            lines[index].end_line(start_char, end_char)
            index += 1
            lines.append(Line(self.font))
            lines[index].add_word(word)

        lines[index].end_line(start_char, end_char)

        logger.info(f"Text split into {len(lines)} lines")
        return lines, start_char_number_of_strokes, end_char_number_of_strokes
