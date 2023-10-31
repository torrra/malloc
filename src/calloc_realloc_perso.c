#include <unistd.h>

#include "malloc_perso.h"
#include "free_perso.h"
#include "calloc_realloc_perso.h"

void* realloc_perso(void* to_realloc, size_t new_size)
{
    free_perso(to_realloc);

    char*   new_data = (char*) malloc_perso(new_size);

    if (NULL == new_data)
        return NULL;

    char    new_data_copy[((t_block*) to_realloc - 1)->m_size];

    to_realloc = (char*) to_realloc;

    for (size_t index = 0; index < ((t_block*) ((__intptr_t) to_realloc + (sizeof(t_block))))->m_size && 
    index < new_size; index++)
    {
        new_data_copy[index] = *((char*)to_realloc);
        to_realloc = ((char*) to_realloc) + 1;
    }
    to_realloc = NULL;
    *new_data = *new_data_copy;
    return new_data;
}

void* calloc_perso(size_t nb_elements, size_t size)
{
    char*    block_data = (char*) malloc_perso(nb_elements * size);

    if (NULL == block_data)
        return NULL;
    for (size_t index = 0; index < size * nb_elements; index++)
    {
        *(block_data + index) = 0;
    }
    return (void*) block_data;
}
