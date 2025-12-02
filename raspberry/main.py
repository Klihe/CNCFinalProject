import streamlit as st
from modules.machine import Machine

machine = Machine()

st.title("Text to Arduino Drawing with Visualization")
text_input = st.text_input("Enter text to draw:", "HELLO")

if st.button("Draw"):
    if not text_input:
        st.warning("Enter some text!")
    else:
        machine.write_text(text_input)
        st.success("Drawing finished!")
