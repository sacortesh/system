#include "processus.h"
#include "cpu.h"
#include "debug.h"
#include "string.h"
#include "malloc.c.h"
#include "timer.h"




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
    activable,
    endormi,
    mourant
};

typedef struct processus {
    char nom[MAXNOMTAILLE];
    int pid;
    int etat;
    int initialisee;

    /*
    0->ebx
    1->esp
    2->ebp
    3->esi
    4->edi
     */
    unsigned long registres[REQ_REG];
    unsigned long stack[STCK_PROC];
    unsigned int reveiller;
    struct processus* suiv;
} processus;

typedef struct list_processus {
    processus* tete;
    processus* queue;
} list_processus;

processus* actif;
list_processus activables;
list_processus endormis;
list_processus mourants;

int pid_courante = 0;
int procs_courantes = 0;

void ajouter_en_tete(processus* proc, list_processus* liste) {
    proc->suiv = NULL;
    if (liste->tete == NULL) {
        liste->tete = proc;
        liste->queue = proc;
    } else {
        processus* tmp = liste->tete;
        liste->tete = proc;
        liste->tete->suiv = tmp;
    }
}

void ajouter_en_queue(processus* proc, list_processus* liste) {
    proc->suiv = NULL;
    if (liste->tete == NULL) {
        liste->tete = proc;
        liste->queue = proc;
    } else {
        liste->queue->suiv = proc;
        liste->queue = liste->queue->suiv;
    }
}

void ajouter_par_priorite(processus* proc, list_processus* liste) {
    proc->suiv = NULL;
        
    //Fait simplement pour reveiller un processus, avec le membre reveiller
    if (liste->tete == NULL) {
        liste->tete = proc;
    } else {

        processus* gauche;
        processus* droit;

        droit = liste->tete;

        int priorite = proc->reveiller;

        //Cas dans lequel le processus a reveiller est avant que la tete de la liste
        if (priorite < droit->reveiller) {
            proc->suiv = droit;
            liste->tete = proc;
            return;
        }

        //On trouve le place potentielle ou stocker le processus
        while(droit->suiv != NULL){
            if(priorite >= droit->reveiller){
                gauche = droit;
                droit = droit->suiv;
            }else{
                break;
            }
        }

        if(droit->suiv == NULL && priorite>=droit->reveiller){
            droit->suiv = proc;
            return;
        }
        
        gauche->suiv = proc;
        gauche = proc;
        gauche->suiv = droit;
        return;

        
        /*
        processus* courante = liste->tete;
        processus* ante_courante;

        
        if (proc->reveiller < courante->reveiller) {
            proc->suiv = courante;
            liste->tete = proc;
            return;
        }


        while (courante != NULL && courante->reveiller < proc->reveiller) {
            ante_courante = courante;
            courante = courante->suiv;
        }

        if (ante_courante == NULL) {
            printf("ERROR: Antecourante not init");
        }

        ante_courante->suiv = proc;

        if (courante != NULL) {
            proc->suiv = courante;
        } else {
            liste->queue = proc;
        }
        */
    }

}

void print_liste(list_processus* liste) {
    if (liste->tete == NULL) {
        printf("Aucune processus dans cette liste\n");
        return;
    }

    processus* cour = liste->tete;
    while (1){
        printf("{%i [%s] | Rev = %u }-> ", cour->pid, cour->nom, cour->reveiller);
        if(cour->suiv != NULL){
            cour = cour->suiv;
        }else {
            break;
        }
        
    }

    printf("NULL\n");

}

void print_status() {
    printf("\f");
    printf("Actif:\n");
    printf("{%i [%s] | Status = %i }\n", actif->pid, actif->nom, actif->etat);
    printf("Activables:\n");
    print_liste(&activables);
    printf("Endormis:\n");
    print_liste(&endormis);
    printf("Mourants:\n");
    print_liste(&mourants);
}

processus* retirer_de_tete(list_processus* liste) {
    if (liste->tete == NULL) {
        return NULL;
    } else {
        processus* tmp = liste->tete;
        liste->tete = liste->tete->suiv;
        tmp->suiv = NULL;
        return tmp;
    }
}

/* Fonction pour nettoyer la structure des donnes des processus
*/
void destroy_processus(processus* proc){
    strcpy(proc->nom, "");
    proc->pid = -1;
    proc->etat = -1;
    proc->reveiller = -1;
    proc->suiv = NULL;
}

/*Cette fonction sert a faire un recyclage des structures de processus qui ne seront plus utilisees
*/
void fin_processus(){
    procs_courantes--;
    processus* ancien = actif;
    ancien->reveiller = -1;
    ancien->etat = mourant;
    ordonnance();
}

int cree_processus(void (*code)(void), char *nom) {
    //initialitation
    if (procs_courantes < NUMPROC){
        procs_courantes++;
        int pid = pid_courante++;

        processus * tmp;
        if (mourants.tete == NULL){
            tmp = (processus *) malloc(sizeof (processus));
        }else{
            tmp = retirer_de_tete(&mourants);
            destroy_processus(tmp);
        }

        strcpy(tmp->nom, nom);
        tmp->etat = activable;
        tmp->pid = pid;

        tmp->registres[1] = (unsigned long) &tmp->stack[STCK_PROC - 3];
        tmp->stack[STCK_PROC - 3] = (unsigned long) code;
        tmp->stack[STCK_PROC - 2] = (unsigned long) fin_processus;
        tmp->reveiller = -1;
        ajouter_en_queue(tmp, &activables);

        return pid;
    }else{
        printf("Plus de processus impossible\n");
        return -1;
    }

}

int cree_processus_idle(char *nom) {
    //initialitation

    int pid = pid_courante++;
    procs_courantes++;

    processus * tmp;
    if (mourants.tete == NULL){
        tmp = (processus *) malloc(sizeof (processus));
    }else{
        tmp = retirer_de_tete(&mourants);
        destroy_processus(tmp);
    }

    strcpy(tmp->nom, nom);
    tmp->etat = elu;
    tmp->pid = pid;
    tmp->reveiller = -1;


    //Initialiser le processus actif
    actif = tmp;


    return pid;

}


int mon_pid(void) {
    return actif->pid;
}

char* mon_nom(void) {
    return actif->nom;
}
//Debugging fonction; Je l'appelle quand un valeur, comme l'heure arrive a quelque valeur specifique
void breaking_bad(void){
    //print_status();
}

void ordonnance(void) {

    print_status();

    int t_actuel = nbr_secondes();
    

    if  (t_actuel == 4) breaking_bad();

    //On reveille tous qui doivent se reveiller
    processus* tmp;
    while (1) {
        if (endormis.tete != NULL && endormis.tete->reveiller == t_actuel) {
            tmp = retirer_de_tete(&endormis);
            tmp->etat = activable;
            tmp->reveiller = -1;
            ajouter_en_queue(tmp, &activables);
            printf("ordonnanceur essais de ajouter %s a la queue\n", tmp->nom);
        } else break;
    }

    //On gestione le prochain status du processus actif
    if (actif->etat == endormi){
        ajouter_par_priorite(actif, &endormis);        
    }else if(actif->etat == mourant){
        ajouter_en_queue(actif, &mourants);
    }else if (actif->etat == elu){
        actif->etat = activable;
        ajouter_en_queue(actif, &activables);
    }else{
        printf("DAFUQ\n");
    }

    processus *ancien, *nouveau;
    ancien = actif;
    nouveau = retirer_de_tete(&activables);
    nouveau->etat = elu;
    actif = nouveau;

    print_status();
    ctx_sw(ancien->registres, nouveau->registres);

}

void dors(unsigned int nbr_secs) {
    processus* ancien = actif;
    ancien->reveiller = nbr_secs + nbr_secondes();
    ancien->etat = endormi;
    ordonnance();
}

void idle(void) {
    /* TEST1 et 2
        for (int i = 0; i < 3; i++) {
            printf("[idle] je tente de passer la main a proc1...\n");
            ctx_sw(&t_processus[0]->registres, &t_processus[1]->registres);
            printf("[idle] proc1 m'a redonne la main\n");
        }
        printf("[idle] je bloque le systeme\n");
        hlt();
     */

    /*Test 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */

    /*Test 4
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100 * 1000 * 1000; i++);
            sti();
            hlt();
            cli();
        }
     */

    for (;;) {
        sti();
        hlt();
        cli();
    }

}

void proc3(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100 * 1000 * 1000; i++);
            sti();
            hlt();
            cli();
        }
     */

        /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */

    //TEST 5: Dors de processus en desordre
    /*
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }*/

    //Test 6
        /*
    for (int i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }
    fin_processus();
    */
    for (int i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(5);
    }

    cree_processus(proc3, "proc3");
   
    
}

void proc1(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*
        TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100 * 1000 * 1000; i++);
            sti();
            hlt();
            cli();
        }
     */

        /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */
    /*TEST 5: Dors de processus en desordre
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(10);
    }
    */

    //TEST 6: Fin des processus
    /*
    for (int i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
    }
    fin_processus();
*/

    for (int i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(2);
    }

    cree_processus(proc3, "proc3");

}



void proc2(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100 * 1000 * 1000; i++);
            sti();
            hlt();
            cli();
        }
     */

        /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */
    //TEST 5: Dors de processus en desordre
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(), mon_nom(), mon_pid());
        dors(3);
    }

   

}

void proc4(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */

    /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */

}

void proc5(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */

}

void proc6(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */

}

void proc7(void) {
    /*TEST1 et 2
        for (;;) {
            printf("[proc1] idle m'a donne la main\n");
            printf("[proc1] je tente de lui la redonner...\n");
            ctx_sw(&t_processus[1]->registres, &t_processus[0]->registres);
        }
        hlt();
     */

    /*
    TEST 3
        for (;;) {
            printf("[%s] pid = %i\n", mon_nom(), mon_pid());
            for (int i = 0; i < 100000000; i++);
            ordonnance();
        }
     */
    /*TEST 4: Generalisation a n processus
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        for (int i = 0; i < 100 * 1000 * 1000; i++);
        sti();
        hlt();
        cli();
    }
    */

}

void init_processus() {



    /* pour proc1 la case de la zone de sauvegarde des registres correspondant à %esp
     * doit pointer sur le sommet de pile, et la case en sommet de pile doit contenir 
     * l'adresse de la fonction proc1 : c'est nécessaire comme expliqué plus haut pour 
     * gérer la première exécution de proc1.  Quoi!?!?!?!?!
     */

    cree_processus_idle("idle");
    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3");


    /* TEST Necessaires
        cree_processus(proc4, "proc4");
        cree_processus(proc5, "proc5");
        cree_processus(proc6, "proc6");
        cree_processus(proc7, "proc7");
     */


}


