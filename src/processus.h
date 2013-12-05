/* 
 * File:   processus.h
 * Author: sparrow
 *
 * Created on 24 November 2013, 20:25
 */



#ifndef PROCESSUS_H
#define	PROCESSUS_H

#define MAXNOMTAILLE 13
#define REQ_REG 5
#define REQ_REG 5
#define STCK_PROC 512
#define QUANPROC 2



extern void ctx_sw(void * courante_proc, void * nouveau_proc);
void idle(void);
void init_processus();
void ordonnance(void);
void print_status();






#endif	/* PROCESSUS_H */

