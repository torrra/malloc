#include <unistd.h>

#include "allocate.h"

void* reallocate(void* to_realloc, size_t new_size)
{
    // Free block to either split it if large enough or leave it free if not
    deallocate(to_realloc);

    // Cast new data pointer to char* for later pointer arithmetics (since char is 1 byte)
    char*   new_data = (char*) allocate(new_size);

    // Return NULL if allocation failed
    if (NULL == new_data)
        return NULL;

    // Use array of block size to get each byte of data
    char    new_data_copy[((t_block*) to_realloc - 1)->m_blockSize];

    // For each byte, copy it into the array and increment pointer to copy next byte
    for (size_t index = 0; index < ((t_block*) ((char*) to_realloc + (sizeof(t_block))))->m_blockSize && 
    index < new_size; index++)
    {
        new_data_copy[index] = *((char*)to_realloc);
        to_realloc = ((char*) to_realloc) + 1;
    }

    // Set argument to NULL
    to_realloc = NULL;

    // Copy data from array into new data pointer
    *new_data = *new_data_copy;

    // Return new data pointer
    return new_data;
}

void* array_alloc(size_t nb_elements, size_t size)
{
    // Cast new data pointer to char* for pointer arithmetics
    char*    block_data = (char*) allocate(nb_elements * size);

    // Return NULL if allocation failed
    if (NULL == block_data)
        return NULL;
    
    // Initialize nb_elements * size bytes to 0
    for (size_t index = 0; index < size * nb_elements; index++)
        *(block_data + index) = 0;

    // Return data array as pointer to void
    return (void*) block_data;
}
