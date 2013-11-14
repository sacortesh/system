/* 
 * File:   screen.c
 * Author: corteshs:Sergio Andres CORTES HERNANDEZ
 *
 * Created on 28 September 2013, 22:40
 */

#include "string.h"
#include "cpu.h"
#include "stdint.h"
#include "console.h"
#include "debug.h"


/*
 * 
 */

#define NUM_COL 80
#define NUM_LIG 25
#define OS_BCK_COLOR 4 //BlOOD OF KINGU
#define OS_FNT_COLOR 15

int cur_lig_pos = 0;
int cur_col_pos = 0;

unsigned short *ptr_mem(unsigned lig, unsigned col) {
    return (unsigned short *) (0xB8000 + 2 * (lig * NUM_COL + col));

}


void ecrit_car(unsigned lig, unsigned col, char c, unsigned color, unsigned fond) {
    //ct
    unsigned short * ptr = ptr_mem(lig, col);
    //1 llamar a metodo que me dqrq la direccion donde guqrdar el caracter
    *ptr = c | (0 << 15) | (fond << 12) | (color << 8);

}

void defilement(void) {
    // il faut verifier que les movement soient bien fait sans effacer le status bar
    memmove((void *) 0xB8000, (void *) 0xB8000 + 2 * (1 * 80 + 0), (size_t) 4000);
    for (int x = 0; x < NUM_COL; x++) {
        ecrit_car(NUM_LIG - 1, x, ' ', OS_FNT_COLOR, OS_BCK_COLOR);
    }

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
            ecrit_car(y, x, ' ', OS_FNT_COLOR, OS_BCK_COLOR);
        }
    }

}

void traite_car(unsigned char c) {

    //cast to ascii code
    int ch = (int) c;

    if (ch <= 126 && ch >= 32) {

        ecrit_car(cur_lig_pos, cur_col_pos, c, OS_FNT_COLOR, OS_BCK_COLOR);

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

void prnt_horloge(int heure, int minute, int seconde) {
    char format[5];
    char temp[2];

    //temp = integer_to_array(heure, temp);


    if (heure < 10) {
        sprintf(temp, "0%d", heure);
    } else {
        sprintf(temp, "%d", heure);
    }

    format[0] = temp[0];
    format[1] = temp[1];
    
    
    if (heure < 10) {
        sprintf(temp, "0%d", minute);
    } else {
        sprintf(temp, "%d", minute);
    }

    format[2] = temp[0];
    format[3] = temp[1];

    if (heure < 10) {
        sprintf(temp, "0%d", seconde);
    } else {
        sprintf(temp, "%d", seconde);
    }

    format[4] = temp[0];
    format[5] = temp[1];
    //TODO finish heure, minute, seconde... then test then profit    

    ecrit_car(0, 72, format[0], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 73, format[1], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 74, 'h', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 75, format[2], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 76, format[3], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 76, 'm', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 77, format[4], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 78, format[5], OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 79, 's', OS_BCK_COLOR, OS_FNT_COLOR);


}

void prnt_status() {

    for (int x = 0; x < NUM_COL; x++) {
        ecrit_car(0, x, ' ', OS_BCK_COLOR, OS_FNT_COLOR);
    }

    ecrit_car(0, 1, 'X', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 2, 'u', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 3, 'r', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 4, 'X', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 5, 'O', OS_BCK_COLOR, OS_FNT_COLOR);
    ecrit_car(0, 6, 'S', OS_BCK_COLOR, OS_FNT_COLOR);

    //Possibly useless line
    prnt_horloge(0, 0, 0);
}

void ecran_init() {

    efface_ecran();
    prnt_status();
    place_curseur(1, 0);
}




