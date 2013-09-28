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

void place_curseur(unsigned lig, unsigned col) {

    cur_lig_pos = lig;
    cur_col_pos = col;

    unsigned short pos = (lig * NUM_COL) + col;

    // bas
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (pos & 0xFF));
    // haut
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((pos >> 8)&0xFF));

}

void ecrit_car(unsigned lig, unsigned col, char c, unsigned color, unsigned fond) {

    unsigned short * ptr = ptr_mem(lig, col);
    //1 llamar a metodo que me dqrq la direccion donde guqrdar el caracter
    *ptr = c | (0 << 15) | (fond << 12);

    //(tambien se puede cl<<7|(coulour_fond)<< 4 (ou 9)| coulour_texte) y hacer otras sumas con la direccion


}

unsigned short *ptr_mem(unsigned lig, unsigned col) {
    return (unsigned short) 0xB8000 + (lig * NUM_COL + col);

}

void efface_ecran(void) {
    //hacer una funcion que escriba caracteres en blancos. Luego hacerlo recorrer por todo el tablero.

    for (int y = 0; y < NUM_LIG; y++) {
        for (int x = 0; x < NUM_COL; x++) {
            ecrit_car(x, y, ' ', 15, 0);
        }
    }

    place_curseur(0, 0);

}

void traite_car(char c) {

    //cast to ascii code
    int ch = (int) c;

    if (ch <= 126 && ch >= 32) {

        ecrit_car(cur_lig_pos, cur_col_pos, c, 15, 0);
        if (cur_lig_pos < NUM_COL) {
            place_curseur(cur_lig_pos + 1, cur_col_pos);

        } else {
            place_curseur(0, cur_col_pos + 1);
        }

    }

    if (ch == 8) {
        //Alors... BS
        if (cur_lig_pos != 0) {
            place_curseur(cur_lig_pos - 1, cur_col_pos);
        }
    }
    if (ch == 9) {
        //Alors... HT 1, 9, 17, ..., 65, 73, 80

        int new_x = cur_lig_pos - (cur_lig_pos % 8) + 8;

        place_curseur(new_x, cur_col_pos);

    }
    if (ch == 10) {
        //Alors... LF

        place_curseur(0, cur_col_pos + 1);

    }
    if (ch == 12) {
        //TODO
        efface_ecran();
    }
    if (ch == 13) {
        //TODO
        place_curseur(0, cur_col_pos);
    }




}

void defilement(void) {

    memmove((void *) 0xB8000, (void *) 0xB8000 + 2 * (1 * 80 + 0), (size_t) 4000);

}

void console_putbytes(char *chaine, int32_t taille) {
    int rd = 0;
    while (rd < taille) {
        traite_car(chaine[rd]);
        rd++;
    }
    return 0;
}



