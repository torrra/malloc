#ifndef MALLOC_PERSO_H
#define MALLOC_PERSO_H

#include <stdbool.h>
#include <stddef.h>

#define ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))
// typedef unsigned long size_t;

// Store block metadata on the heap
typedef struct s_block
{
    size_t              m_size;
    struct s_block*     m_prev;
    struct s_block*     m_next;
    bool                m_free;
}t_block;

extern void* g_head ;

void* malloc_perso(size_t size);

t_block* extend_heap(size_t size);

void initialize_block(t_block* block);

t_block* find_block(size_t size);

t_block* split_block (t_block* b, size_t size);

void try_to_fusion(t_block* freed_block);

#endif