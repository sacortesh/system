#include "stdio.h"
#include "timer.h"

int cpt;
int heure;
int minute;
int seconde;

void init_horloge() {
    cpt = heure = minute = seconde = 0;

    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
    outb((QUARTZ / CLOCKFREQ) >> 8, 0x40);
    //interruption


}

void tic_PIT() {
    cpt++;

    if ((cpt % 50 == 0)) {
        seconde++;
        if (seconde == 60) {
            seconde == 0;
            minute++;
            if (minute == 60) {
                minute == 0;
                heure++;
            }
        }
    }

}

