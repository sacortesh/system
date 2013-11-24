#include <cpu.h>
#include "debug.h"
#include "console.h"
#include "timer.h"
#include "interruptions.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue



/*
unsigned fact(unsigned n) {
    unsigned res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}
*/

void kernel_start(void) {
    //unsigned x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    //(void) x;
    // on ne doit jamais sortir de kernel_start
    ecran_init();
    
    init_horloge();
    init_traitant_IT(32, traitant_IT_32);
    
    
    
    printf("Hola... es hora de probar si esto funciona bien. "
            "Primero una linea muy muy larga..................."
            "................................................"
            "..............");
    printf(".\nFin de linea ? .... Hasta aqui todo va bien.... "
            "ahora muchas tabulaciones\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
            "\t\t");
    printf(" ok.... y ahora...Salto\b\b me gusta\nXXXX"
            "XXXXXXXXXXXXXXXXXXX\rY");
    idle();





    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

