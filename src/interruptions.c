#include "interruptions.h"
#include "stdbool.h"
#include "cpu.h"
#include "segment.h"

unsigned short * TVI = (unsigned short *) 0x1000;

void init_traitant_IT(int num_IT, void (*traitant)(void)) {
    unsigned short * temp = TVI + (num_IT * 4);

    *temp = (int) traitant;
    *(temp + 1) = KERNEL_CS;
    *(temp + 2) = 0x8E00;
    *(temp + 3) = (int) traitant >> 16;
}

void masque_IRQ(unsigned int num_IRQ, bool masque) {

    unsigned char val = inb(0x21);

    if (masque) {
        val = val | (1 << num_IRQ);
    } else {
        val = val & ~(1 << num_IRQ);
    }

    outb(val, 0x21);
}

