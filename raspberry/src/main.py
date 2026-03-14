import time

import streamlit as st
from loguru import logger

from modules.machine import Machine

if "machine" not in st.session_state:
    logger.info("First run — creating Machine instance")
    st.session_state.machine = Machine()
    logger.info("Waiting 8s for Arduino to boot")
    time.sleep(8)
    logger.success("Machine initialized and ready")

machine = st.session_state.machine

st.title("CNC Plotter")
text_input = st.text_area("", placeholder="Enter text to draw:", label_visibility="collapsed")

if st.button("Draw"):
    if not text_input:
        logger.warning("Draw clicked with empty input")
        st.warning("Enter some text!")
    else:
        logger.info(f"Draw requested: {text_input!r}")
        machine.write_text(text_input)
        logger.success("Drawing finished")
        st.success("Drawing finished!")

