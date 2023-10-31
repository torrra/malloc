#ifndef CALLOC_REALLOC_H
#define CALLOC_REALLOC_H

#include "malloc_perso.h"

/* Reallocate (resize and possibly move) a block only if non null 
and previously allocated with malloc_perso. If the pointer passed to realloc_perso
is invalid, this function is equivalent to malloc_perso */
void* realloc_perso(void* to_realloc, size_t size);

// Allocate an aray of n elements of x bytes on the heap
void* calloc_perso(size_t nb_elements, size_t new_size);

#endif