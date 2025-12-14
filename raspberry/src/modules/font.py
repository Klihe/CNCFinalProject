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
        self.width = 0
        self.text = ""

    def add_word(self, word: str) -> bool:
        word_width = self._word_width(word)
        if word_width + self.width > Const.SLICING.LINE_WIDTH:
            return False

        self.width += word_width
        self.text += word

        return True

    def _word_width(self, word) -> float:
        if not word:
            return 0.0

        lines = self.font.lines_for_text(word)

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

    def text_to_strokes(self, text: str) -> list[Line]:
        start_char = "|"
        end_char = "j"

        words: list[str] = re.findall(r"\S+\s*", text)
        words.insert(0, start_char)
        words.append(end_char)

        lines = [Line(self.font)]
        index = 0

        for word in words:
            if lines[index].add_word(word):
                continue
            index += 1
            lines.append(Line(self.font))
            lines[index].add_word(word)

        return lines
