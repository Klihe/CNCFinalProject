from HersheyFonts import HersheyFonts

from const.const import Const
import re

class Font(HersheyFonts):
    def __init__(self) -> None:
        super().__init__()
        self.load_default_font(Const.FONT.NAME)

class Line:
    def __init__(self, font: Font):
        self.font = font
        self.text = ""

    def add_word(self, word: str) -> bool:
        text_width = self._text_width(self.text + word)
        if text_width > Const.SLICING.LINE_WIDTH:
            return False

        self.text += word
        return True

    def end_line(self, start_char, end_char) -> None:
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
        self.font = Font()

    def text_to_strokes(self, text: str) -> tuple[list[Line], int, int]:
        start_char = "|"
        end_char = "j"

        start_char_number_of_strokes = len(list(self.font.lines_for_text(start_char)))
        end_char_number_of_strokes = len(list(self.font.lines_for_text(end_char)))

        words: list[str] = re.findall(r"\S+\s*", text)

        lines = [Line(self.font)]
        index = 0

        for word in words:
            if lines[index].add_word(word):
                continue
            lines[index].end_line(start_char, end_char)
            index += 1
            lines.append(Line(self.font))
            lines[index].add_word(word)

        return lines, start_char_number_of_strokes, end_char_number_of_strokes
