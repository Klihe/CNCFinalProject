from modules.machine import Machine
import time

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(8)

    machine.write_text("""Utrpeni na elektrotechnicke prumyslovce Jecna Praha 2 ma mnoho podob, ale pribeh studenta Stepana Brizi je vyjimecny. Tri dny nespal, protoze se pripravoval na test od pana Budaie. Oci mel cervene, hlavu plnou vzorcu a mozek jel uz jen na silu vune a kofeinu. Kazda minuta byla boj mezi vedomim a spankem. Ve skole sedel jako telo bez duse, ale presto bojoval dal, protoze vedel, ze selhani neni moznost. Test se stal symbolem psychicke zkousky, ne jen znalosti.""")
