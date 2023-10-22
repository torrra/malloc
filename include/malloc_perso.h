#ifndef MALLOC_PERSO_H
#define MALLOC_PERSO_H

#include <stdbool.h>

// typedef unsigned long size_t;

// Store block metadata on the heap
typedef struct s_block
{
    size_t              m_size;
    struct s_block*     m_prev;
    struct s_block*     m_next;
    bool                m_free;
}t_block;

typedef struct s_footer
{
    size_t m_data_size;
}t_footer;

void* fragment_block(t_block* big_block, size_t to_alloc);

#endif