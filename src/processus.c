#include "processus.h"
#include "cpu.h"
#include "debug.h"
#include "string.h"
#include "malloc.c.h"


//TODO: TP3

/*
 * Structure des donnees pour le processus
 * son numéro, qui servira d'identifiant unique dans le système, et que l'on appelle typiquement pid (pour Process IDentifier) ;
son nom, une chaine de caractères qui servira à produire des traces lisibles ;
son état : un processus peut être « élu » (c'est à dire que c'est lui que le processeur est en train d'exécuter), « activable » (c'est à dire qu'il est prêt à être élu et qu'il attend que le processeur se libère), « endormi » pour une durée donnée, etc. ;
son contexte d'exécution sur le processeur : il s'agit en pratique d'une zone mémoire servant à sauvegarder le contenu des registres important du processeur lorsqu'on arrête le processus pour passer la main à un autre, et à restaurer ce contenu ensuite lorsque le processus reprend la main ;
sa pile d'exécution : qui est l'espace mémoire dans lequel sont stockés notamment les variables locales, les paramètres passés aux fonctions, etc..
 * Tableau des processus
 * Pointeur de processus elu
 * Ordonnenceur mode tourniquet
 * Sauvegardement de letat
 * Les numeros magiques a constantes
 */



enum status {
    elu,
    activable
};

typedef struct processus {
    char nom[MAXNOMTAILLE];
    int pid;
    int etat;
    unsigned long* esp[REQ_REG];
    unsigned long stack[QUANPROC];
} processus;

processus t_processus[2];

void idle(void) {
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(t_processus[0].esp[0], t_processus[1].esp[0]);
}

void proc1(void) {
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
}

void init_processus() {
    //Allocation

    //TODO:
    //Les erreurs

    //idle
        
    strcpy(t_processus[0].nom, "idle");
    t_processus[0].etat = elu;
    t_processus[0].pid = 0;
    //proc1


    /* pour proc1 la case de la zone de sauvegarde des registres correspondant à %esp
     * doit pointer sur le sommet de pile, et la case en sommet de pile doit contenir 
     * l'adresse de la fonction proc1 : c'est nécessaire comme expliqué plus haut pour 
     * gérer la première exécution de proc1.  Quoi!?!?!?!?!
     */

    
    strcpy(t_processus[1].nom, "proc1");
    t_processus[1].etat = activable;
    t_processus[1].pid = 1;

    t_processus[1].esp[0] = &t_processus[1].stack[0];
    t_processus[1].stack[0] = (unsigned long) &proc1  ;


}


