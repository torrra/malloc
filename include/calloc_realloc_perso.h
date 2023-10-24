#ifndef CALLOC_REALLOC_H
#define CALLOC_REALLOC_H

#include "malloc_perso.h"

void* realloc_perso(void* to_realloc, size_t size);

void* calloc_perso(size_t nb_elements, size_t new_size);

#endif