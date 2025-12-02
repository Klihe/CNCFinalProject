import time
from math import sqrt

import streamlit as st

from const import Const
from modules.commands import Commands
from modules.font import Font

commands = Commands()
font = Font()

# --- Helper functions ---

def send_relative_move(dx, dy, chunk_size=Const.DATA.CHUNK_SIZE):
    """Send relative move to Arduino in chunks with sleep proportional to distance."""
    while abs(dx) > 0 or abs(dy) > 0:
        # Determine chunk
        chunk_dx = max(-chunk_size, min(chunk_size, dx))
        chunk_dy = max(-chunk_size, min(chunk_size, dy))

        # Send move to Arduino
        commands.send_chunk(chunk_dx, chunk_dy)

        # Update remaining move
        dx -= chunk_dx
        dy -= chunk_dy

def distance(p1, p2):
    """Euclidean distance."""
    return sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

# --- Streamlit UI ---
st.title("Text to Arduino Drawing with Visualization")

text_input = st.text_input("Enter text to draw:", "HELLO")


if st.button("Draw"):
    if not text_input:
        st.warning("Enter some text!")
    else:
        # --- Initialize CNC state ---
        commands.pen_up()
        current_x, current_y = 0, 0
        pen_is_down = False
        last_pen_action_pos = (0, 0)

        # --- Draw all strokes ---
        for stroke in font.text_to_strokes(text_input):
            if len(stroke) < 1:
                continue

            x0, y0 = stroke[0]
            dist_to_last = distance(last_pen_action_pos, (x0, y0))

            # --- PEN UP if needed ---
            if pen_is_down and dist_to_last > Const.SLICING.DIST_THRESHOLD:
                commands.pen_up()
                pen_is_down = False

            # --- Move to start of stroke ---
            dx = int((x0 - current_x) * Const.FONT.STEP_MULTIPLIER)
            dy = int((y0 - current_y) * Const.FONT.STEP_MULTIPLIER)
            send_relative_move(dx, dy, chunk_size=Const.DATA.CHUNK_SIZE)
            current_x, current_y = x0, y0

            # --- PEN DOWN if needed ---
            if not pen_is_down:
                commands.pen_down()
                pen_is_down = True

            last_pen_action_pos = (current_x, current_y)

            # --- Draw the stroke ---
            draw_path = [(current_x, current_y)]
            for x, y in stroke[1:]:
                dx = int((x - current_x) * Const.FONT.STEP_MULTIPLIER)
                dy = int((y - current_y) * Const.FONT.STEP_MULTIPLIER)
                send_relative_move(dx, dy, chunk_size=Const.DATA.CHUNK_SIZE)
                current_x, current_y = x, y
                draw_path.append((current_x, current_y))
                last_pen_action_pos = (current_x, current_y)


        # --- PEN UP at the end ---
        if pen_is_down:
            commands.pen_up()
            pen_is_down = False

        st.success("Drawing finished!")
