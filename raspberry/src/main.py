import time

import streamlit as st
from loguru import logger

from modules.machine import Machine

if "machine" not in st.session_state:
    logger.info("First run — creating Machine instance")
    st.session_state.machine = Machine("/dev/tty.usbserial-110")
    logger.info("Waiting 8s for Arduino to boot")
    time.sleep(8)
    logger.success("Machine initialized and ready")

machine = st.session_state.machine

st.title("CNC Drawing Machine")
col1, col2 = st.columns(2)
with col1:
    page1 = st.text_area("Page 1", height=720, key="page1")
with col2:
    page2 = st.text_area("Page 2", height=720, key="page2")
if st.button("Draw"):
    if not page1 and not page2:
        logger.warning("Draw clicked with empty input")
        st.warning("Enter some text!")
    else:
        if page1:
            logger.info(f"Drawing page 1: {page1!r}")
            machine.write_text(page1)
        if page2:
            logger.info("Advancing to page 2")
            machine.commands.next_page()
            logger.info(f"Drawing page 2: {page2!r}")
            machine.write_text(page2)
        logger.success("Drawing finished")
        st.success("Drawing finished!")
