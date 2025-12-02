from HersheyFonts import HersheyFonts

from const import Const


class Font:
    def __init__(self) -> None:
        self._font_name: str = Const.FONT.NAME
        self._font_scale: float = Const.FONT.SCALE
        self._font_spacing: float = Const.FONT.SPACING

        self._font = HersheyFonts()
        self._font.load_default_font(self._font_name)

    def change_font_name(self, font_name: str) -> None:
        self._font_name = font_name

    def change_font_scale(self, font_scale: float) -> None:
        self._font_scale = font_scale

    def change_font_spacing(self, font_spacing: float) -> None:
        self.font_spacing = font_spacing

    def text_to_strokes(self, text: str) -> list:
        lines = self._font.lines_for_text(text)
        all_strokes = []
        for line in lines:
            if line:
                scaled_line = [(y * self._font_scale, -x * self._font_scale) for x, y in line]
                all_strokes.append(scaled_line)
                x_offset += self.font_spacing * self._font_scale

        return all_strokes
