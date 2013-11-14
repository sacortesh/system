/* 
 * File:   screen.c
 * Author: corteshs:Sergio Andres CORTES HERNANDEZ
 *
 * Created on 28 September 2013, 22:40
 */

#include "string.h"
#include "cpu.h"
#include "stdint.h"


/*
 * 
 */

#define NUM_COL 80
#define NUM_LIG 25

int cur_lig_pos = 0;
int cur_col_pos = 0;

unsigned short *ptr_mem(unsigned lig, unsigned col) {
    return (unsigned short *) (0xB8000 + 2 * (lig * NUM_COL + col));

}

void defilement(void) {

    memmove((void *) 0xB8000, (void *) 0xB8000 + 2 * (1 * 80 + 0), (size_t) 4000);

}

void ecrit_car(unsigned lig, unsigned col, char c, unsigned color, unsigned fond) {
    //ct
    unsigned short * ptr = ptr_mem(lig, col);
    //1 llamar a metodo que me dqrq la direccion donde guqrdar el caracter
    *ptr = c | (0 << 15) | (fond << 12) | (color << 8);

}

void place_curseur(unsigned lig, unsigned col) {

    cur_lig_pos = lig;
    cur_col_pos = col;

    unsigned int pos = (lig * NUM_COL) + col;

    // bas
    outb((unsigned char) 0x0F, (unsigned short) 0x3D4);
    outb(pos, (unsigned short) 0x3D5);
    // haut
    outb((unsigned char) 0x0E, (unsigned short) 0x3D4);
    outb((unsigned char) (pos >> 8), (unsigned short) 0x3D5);

}

void efface_ecran() {
    //hacer una funcion que escriba caracteres en blancos. Luego hacerlo recorrer por todo el tablero.

    for (int y = 0; y < NUM_LIG; y++) {
        for (int x = 0; x < NUM_COL; x++) {
            ecrit_car(y, x, ' ', 15, 4);
        }
    }

}

void traite_car(unsigned char c) {

    //cast to ascii code
    int ch = (int) c;

    if (ch <= 126 && ch >= 32) {

        ecrit_car(cur_lig_pos, cur_col_pos, c, 15, 0);

        if (cur_lig_pos + 1 < NUM_LIG && cur_col_pos + 1 < NUM_COL) {
            place_curseur(cur_lig_pos, cur_col_pos + 1);
        } else if (cur_col_pos + 1 == NUM_COL) {

            if (cur_lig_pos + 1 < NUM_LIG) {
                place_curseur(cur_lig_pos + 1, 0);
            }

        } else {
            defilement();
            place_curseur(cur_lig_pos, 0);
        }

    } else {

        if (ch == 8) {
            //Alors... BS
            if (cur_col_pos != 0) {
                place_curseur(cur_lig_pos, cur_col_pos - 1);
            }
        }
        if (ch == 9) {
            //Alors... HT 1, 9, 17, ..., 65, 73, 80

            int new_x = cur_col_pos - (cur_col_pos % 8) + 8;
            int new_y = cur_lig_pos;

            if (new_x > NUM_COL) {
                new_x = new_x % 8;
                if (cur_lig_pos + 1 < NUM_LIG) {
                    new_y++;
                } else {
                    defilement();
                }
            }

            place_curseur(new_y, new_x);

        }
        if (ch == 10) {
            //Alors... LF

            if (cur_lig_pos + 1 < NUM_LIG) {
                // on se trouve avant la dernière ligne
                place_curseur(cur_lig_pos + 1, 0);
            } else {
                // on se trouve sur la dernière ligne, défilement à faire
                defilement();
                place_curseur(cur_lig_pos, 0);
            }

        }

        if (ch == 12) {
            efface_ecran();
            place_curseur(0, 0);

        }

        if (ch == 13) {
            place_curseur(cur_lig_pos, 0);
        }


    }






}

void console_putbytes(const char *chaine, int taille) {
    int l = 0;
    while (l < taille) {
        traite_car(chaine[l]);
        l++;
    }
    return;
}



