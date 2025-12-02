import streamlit as st
import serial
import time
import matplotlib.pyplot as plt
from math import sqrt
from HersheyFonts import HersheyFonts

# --- CNC / Arduino Settings ---
ARDUINO_PORT = "/dev/ttyUSB0"
BAUD_RATE = 2_000_000
CHUNK_SIZE = 2000
DIST_THRESHOLD = 1e-3  # Minimum distance to record new pen action
BASE_TIME_PER_UNIT = 0.0001  # seconds per unit (adjust as needed)

# --- Connect to Arduino ---
serial = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)

# --- Load font ---
font = HersheyFonts()
font.load_default_font("futural")

# --- Helper functions ---

def send_relative_move(dx, dy, chunk_size=CHUNK_SIZE):
    """Send relative move to Arduino in chunks with sleep proportional to distance."""
    while abs(dx) > 0 or abs(dy) > 0:
        # Determine chunk
        chunk_dx = max(-chunk_size, min(chunk_size, dx))
        chunk_dy = max(-chunk_size, min(chunk_size, dy))
        
        # Send move to Arduino
        serial.write(f"{chunk_dx},{chunk_dy}\n".encode())
        serial.flush()
        
        # Sleep proportional to distance
        distance_chunk = (chunk_dx**2 + chunk_dy**2)**0.5
        time.sleep(distance_chunk * BASE_TIME_PER_UNIT)
        
        # Update remaining move
        dx -= chunk_dx
        dy -= chunk_dy

def distance(p1, p2):
    """Euclidean distance."""
    return sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

# --- Streamlit UI ---
st.title("Text to Arduino Drawing with Visualization")

text_input = st.text_input("Enter text to draw:", "HELLO")

# --- Fixed parameters ---
font_scale = 0.5
spacing = 0
step_multiplier = 100

if st.button("Draw"):
    if not text_input:
        st.warning("Enter some text!")
    else:
        # --- Convert text to strokes ---
        lines = font.lines_for_text(text_input)
        all_strokes = []
        x_offset = 0
        for line in lines:
            if line:
                scaled_line = [(y * font_scale, -x * font_scale + x_offset) for x, y in line]
                all_strokes.append(scaled_line)
                x_offset += spacing * font_scale

        # --- Initialize CNC state ---
        serial.write(b'PEN_UP\n')
        time.sleep(0.01)
        current_x, current_y = 0, 0
        pen_is_down = False
        last_pen_action_pos = (0, 0)

        # --- Visualization storage ---
        pen_up_paths = []
        pen_down_paths = []
        pen_down_points = []
        pen_up_points = []

        # --- Draw all strokes ---
        for stroke in all_strokes:
            if len(stroke) < 1:
                continue

            x0, y0 = stroke[0]
            dist_to_last = distance(last_pen_action_pos, (x0, y0))

            # --- PEN UP if needed ---
            if pen_is_down and dist_to_last > DIST_THRESHOLD:
                serial.write(b'PEN_UP\n')
                serial.flush()
                time.sleep(0.01)
                pen_is_down = False
                pen_up_points.append(last_pen_action_pos)
                pen_up_paths.append([last_pen_action_pos, (x0, y0)])

            # --- Move to start of stroke ---
            dx = int((x0 - current_x) * step_multiplier)
            dy = int((y0 - current_y) * step_multiplier)
            send_relative_move(dx, dy, chunk_size=CHUNK_SIZE)
            current_x, current_y = x0, y0

            # --- PEN DOWN if needed ---
            if not pen_is_down:
                serial.write(b'PEN_DOWN\n')
                serial.flush()
                time.sleep(0.01)
                pen_is_down = True
                pen_down_points.append((current_x, current_y))

            last_pen_action_pos = (current_x, current_y)

            # --- Draw the stroke ---
            draw_path = [(current_x, current_y)]
            for x, y in stroke[1:]:
                dx = int((x - current_x) * step_multiplier)
                dy = int((y - current_y) * step_multiplier)
                send_relative_move(dx, dy, chunk_size=CHUNK_SIZE)
                current_x, current_y = x, y
                draw_path.append((current_x, current_y))
                last_pen_action_pos = (current_x, current_y)

            pen_down_paths.append(draw_path)

        # --- PEN UP at the end ---
        if pen_is_down:
            serial.write(b'PEN_UP\n')
            serial.flush()
            time.sleep(0.01)
            pen_is_down = False
            pen_up_points.append((current_x, current_y))

        # --- Visualization ---
        fig, ax = plt.subplots()

        # Pen-up moves (red dashed)
        for path in pen_up_paths:
            xs, ys = zip(*path)
            ax.plot(xs, ys, "r--", linewidth=1, label="Pen Up" if "Pen Up" not in ax.get_legend_handles_labels()[1] else "")

        # Pen-down moves (blue solid)
        for path in pen_down_paths:
            xs, ys = zip(*path)
            ax.plot(xs, ys, "b-", linewidth=2, label="Pen Down" if "Pen Down" not in ax.get_legend_handles_labels()[1] else "")

        # Markers for pen-down and pen-up points
        if pen_down_points:
            xs, ys = zip(*pen_down_points)
            ax.scatter(xs, ys, c="green", s=40, marker="o", label="Pen Down Point")
        if pen_up_points:
            xs, ys = zip(*pen_up_points)
            ax.scatter(xs, ys, c="red", s=40, marker="x", label="Pen Up Point")

        ax.set_aspect("equal")
        ax.invert_yaxis()
        ax.legend()
        st.pyplot(fig)

        st.success("Drawing finished!")