#ifndef FREE_PERSO_H
#define FREE_PERSO_H

#include "malloc_perso.h"

/* Deallocate a block only if non null and 
previously allocated with malloc_perso() */
void free_perso (void *ptr);

#endif