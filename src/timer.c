#include "stdio.h"
#include "timer.h"
#include "interruptions.h"
#include "stdbool.h"
#include "console.h"
#include "cpu.h"
#include "processus.h"

int cpt;
int heure;
int minute;
int seconde;

void init_horloge() {
    cpt = heure = minute = seconde = 0;

    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
    outb((QUARTZ / CLOCKFREQ) >> 8, 0x40);
    masque_IRQ(0, false);

}

void tic_PIT() {
    outb(0x20, 0x20);
    cpt++;

    if ((cpt % 50 == 0)) {
        cpt = 0;
        seconde++;
        if (seconde == 60) {
            seconde = 0;
            minute++;
            if (minute == 60) {
                minute = 0;
                heure++;
            }
        }
        
        //print_status();
    }

    prnt_horloge(heure, minute, seconde);
    ordonnance();
}

int nbr_secondes() {
    return (heure * 3600)+(minute * 60) + seconde;
}

