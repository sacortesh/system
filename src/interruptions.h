/* 
 * File:   interruptions.h
 * Author: corteshs
 *
 * Created on 14 novembre 2013, 11:31
 */

#ifndef INTERRUPTIONS_H
#define	INTERRUPTIONS_H

#include "stdbool.h"


void init_traitant_IT(int num_IT, void (*traitant)(void));
void masque_IRQ(unsigned int num_IRQ, bool masque);
extern void traitant_IT_32();

#endif	/* INTERRUPTIONS_H */

