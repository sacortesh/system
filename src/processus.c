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

    /*
    ebx
    esp
    ebp
    esi
    edi
     */
    unsigned long registres[REQ_REG - 1];

    unsigned long stack[STCK_PROC - 1];
} processus;

processus* t_processus[QUANPROC - 1];

processus* actif;

int mon_pid(void) {
    return actif->pid;
}

char *mon_nom(void) {
    return actif->nom;
}

void ordonnance(void) {

    //Je trouve le index du processus à activer


    int nouvProc = 1 - mon_pid();

    actif->etat = activable;
    actif = t_processus[nouvProc];
    actif->etat = elu;

    int ancProc = 1 - mon_pid();


    ctx_sw(&t_processus[ancProc]->registres, &t_processus[nouvProc]->registres);




}

void idle(void) {
    /*
        for (int i = 0; i < 3; i++) {
            printf("[idle] je tente de passer la main a proc1...\n");
            ctx_sw(&t_processus[0]->registres, &t_processus[1]->registres);
            printf("[idle] proc1 m'a redonne la main\n");
        }
        printf("[idle] je bloque le systeme\n");
        hlt();
     */

    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100000000; i++);
        ordonnance();
    }

}

void proc1(void) {
    /*
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100000000; i++);
        ordonnance();
    }

}

void init_processus() {
    //Allocation

    t_processus[0] = malloc(sizeof (processus));
    processus* idlex = t_processus[0];

    //idle

    strcpy(idlex->nom, "idle");
    idlex->etat = elu;
    idlex->pid = 0;

    //Initialiser le processus actif
    actif = idlex;

    /* pour proc1 la case de la zone de sauvegarde des registres correspondant à %esp
     * doit pointer sur le sommet de pile, et la case en sommet de pile doit contenir 
     * l'adresse de la fonction proc1 : c'est nécessaire comme expliqué plus haut pour 
     * gérer la première exécution de proc1.  Quoi!?!?!?!?!
     */

    t_processus[1] = malloc(sizeof (processus));
    processus* procc1 = t_processus[1];


    strcpy(procc1->nom, "proc1");
    procc1->etat = activable;
    procc1->pid = 1;
    //procc1->registres = (unsigned long*) malloc(REQ_REG * sizeof (unsigned long));
    //procc1->stack = (unsigned long*) malloc(STCK_PROC * sizeof (unsigned long));



    procc1->stack[STCK_PROC - 1] = (unsigned long) proc1;
    procc1->registres[1] = (unsigned long) &procc1->stack[STCK_PROC - 1];



}


