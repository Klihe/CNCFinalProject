from modules.machine import Machine
import time

machine = Machine("/dev/tty.usbserial-110")

if __name__ == "__main__":
    time.sleep(8)

    # machine.write_text("""
    #     The series follows main character Guts and his story as a mercenary starting when he is a child. He is born from his mother's dead body, and his adoptive mother dies of plague three years later. The leader of the mercenaries, Gambino, teaches him how to fight. Guts is forced to leave the group he grew up in after killing his drunk father Gambino to defend himself. After losing a duel to Griffith, he joins Griffith's "Band of the Hawk" group. In his time with them he becomes friends with Casca, Judeau, Gaston, Rickert, Pippin, and more. Together, they fight for money and become famous for their success. Guts finds out about a necklace Griffith has called the "Behelit". In one battle, they fight a monster named Nosferatu Zodd who beats Guts and Griffith, but leaves after seeing the Behelit and warns Guts that "Death will pay you a visit". Guts becomes famous for beating an army of 100 soldiers alone.

    #     The band become heroes for winning a war that started a hundred years ago. For this, the king of Midland gives some of them noble titles. Guts hears Griffith say that he can only be friends with someone who has their own ambitions, and he decides to leave the Band of the Hawk after they win the war. Griffith tries to make Guts stay by winning another duel, but he loses and Guts leaves. Griffith feels powerless because he couldn't keep Guts, so he spends the night with the king's daughter. While trying to leave, he is caught by the palace guard and sent to be tortured. The Band of the Hawk are hunted by the army of Midland and Casca becomes their leader. As Guts is resting, he meets a mysterious Skull Knight who gives him an unspecific warning and leaves.

    #     One year later, Casca and the remainder of the Band of the Hawk are saved from a sneak attack by Guts. After Casca fails to kill herself, she and Guts talk about their past to each other and have sex. With the help of Guts, the Band of the Hawk rescues Griffith from prison. Griffith notices that he lost the Behelit, but can not speak because his tongue was cut off. On the orders of the king, they are chased by the Black Dog Knights, an army of criminals lead by Wylad. Wylad turns into his true form as a monster. He almost kills Guts and Griffith, but they are saved by Nosferatu Zodd. When Wylad does, he is sucked into a vortex and his original body as a little old man is revealed. They learn that the king's torture removed Griffith's tongue and many of the muscles he needs to fight.

    #     After seeing a vision, Griffith tries to run away from camp. He falls down and finds the Behelit again. As the band get close to him, the moon begins begins to cover the sun and starts a solar eclipse. The band are trapped inside the eclipse. Four giant demons appear and Griffith is grabbed by a gigantic hand as the rest of the band are attacked by demons. Griffith sacrifices everyone in the eclipse so he can become the fifth member of the "God Hand". Judeau and Pippin die protecting Casca. Guts goes into a fit of rage, but he can't protect Casca from the demons. Griffith, now reborn as Femto, rapes Casca. Guts cuts off his left arm to escape a demon, but he is pinned down and can't do anything. The Skull Knight breaks into the eclipse to save Guts and Casca, but everyone else from the Band of the Hawk die except Rickert, the youngest one who was waiting outside.
    # """)

    machine.commands.next_page()

    for _ in range(30):
        machine.write_text("ahoj")
