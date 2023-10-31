#ifndef MALLOC_PERSO_H
#define MALLOC_PERSO_H

#include <stdbool.h>
#include <stddef.h>

// Align a given integer to the closest superior multiple of 16
#define ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

// Largest value that can be held by a size_t variable (unsigned long)
#define MAX_SIZE_T      4294967295UL

/* Adresses must be aligned on 16 because largest C built-in type
is 16 bytes and variable adresses must be aligned on their type size */
#define ALIGNMENT       16

// Largest unsigned integer type for edge value calculations
typedef unsigned long long large_size_t;

// Store block metadata on the heap
typedef struct s_block
{
    // Size of data (aligned on 16)
    size_t              m_size;
    // Previous malloc_perso heap block
    struct s_block*     m_prev;
    // Next malloc_perso heap block
    struct s_block*     m_next;
    // Free (ready to be allocated) / already in-use status
    bool                m_free;
}t_block;

// Global variable pointing to first heap block
extern void* g_head ;

// Dynamic allocation function
void* malloc_perso(size_t size);

// Push break further up to allocate new blocks on the heap
t_block* extend_heap(size_t size);

// Initialize t_block members to NULL and 0
void initialize_block(t_block* block);

// Find first free block of sufficient size upon malloc call
t_block* find_block(size_t size);

// Fragment block found by find_block if size is too large
t_block* split_block (t_block* b, size_t size);

// Defragment adjacent free blocks if size is not too big
void try_to_fusion(t_block* freed_block);

#endif