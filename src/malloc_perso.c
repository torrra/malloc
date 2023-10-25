#include "malloc_perso.h"
#include <unistd.h>
#include <stdio.h>

void*   g_head = NULL;

void* malloc_perso(size_t size)
{
    size_t  temp_size = size;

    size = ALIGN_UP(size, 16);

    t_block*    found_block = find_block(size);

    if (found_block != NULL)
        return (void*) ++found_block;

    found_block = extend_heap(size);
    printf("I'm going to alloc %zu bytes (sizeof t_block: %zu  + requested size %zu (aligment 16))\
\nThe pointer to block is %p  / block end is %p / so aligned data block is at %p\n\
I found no free block: created a new one %p\n",
    size + sizeof(t_block), sizeof(t_block), temp_size, (void*) found_block, (void*) (found_block + 1), (void*) (found_block + 1), (void*) found_block);
    
    return (void*) ++found_block;
}

t_block* extend_heap(size_t size)
{
    if (sbrk(size + sizeof(t_block)) == (void*) -1)
        return NULL;

    t_block*    new_block = (t_block*) sbrk(0);

    new_block->m_size = size;
    new_block->m_next = NULL;
    new_block->m_free = false;
    if (NULL == g_head)
    {
        g_head = new_block;
        new_block->m_prev = NULL;
    }
    else
    {
        t_block*    list_member = g_head;

        while (NULL != list_member->m_next)
            list_member = list_member->m_next;

        list_member->m_next = new_block;
        new_block->m_prev = list_member;
    }
    return new_block;
}

void initialize_block(t_block* block)
{
    block->m_size = 0;
    block->m_next = NULL;
    block->m_prev = NULL;
    block->m_free = true;
}

t_block* find_block(size_t size)
{
    t_block*    list_member = (t_block*) g_head;

    size = ALIGN_UP(size, 16);
    while (NULL != list_member)
    {
        if (list_member->m_free && list_member->m_size > size + sizeof(t_block))
        {
            printf("I found free block %p\n", (void*) list_member);
            return split_block(list_member, size);
        }
        else if ((list_member->m_free && list_member->m_size == size) || 
        (list_member->m_free && list_member->m_size > size))
        {
            printf("I found free block %p\n", (void*) list_member);
            list_member->m_free = false;
            return list_member;
        }
        list_member = list_member->m_next;
    }
    return NULL;
}

t_block* split_block (t_block* b, size_t size)
{
    t_block*      remainder_block = (t_block*) ((__intptr_t) b + (sizeof(t_block) + size));
    size_t        remainder_size = b->m_size - (size + sizeof(t_block));

    printf("Block %p of size %zu can be split when allocating %zu bytes because block size > \
mimimumSplitBlockSize %zu\nSplitting block %p to make it size %zu and creating \
new block %p having size %zu\n",
    (void*) b, b->m_size, size, size + sizeof(t_block), (void*) b, size, 
    (void*) remainder_block, remainder_size);

    remainder_block->m_prev = b;
    remainder_block->m_next = b->m_next;
    remainder_block->m_size = remainder_size;
    remainder_block->m_free = true;

    b->m_next = remainder_block;
    b->m_size = size;
    b->m_free = false;

    return b;
}

void try_to_fusion(t_block* freed_block)
{
    if (NULL != freed_block->m_next && freed_block->m_next->m_free)
    {
        freed_block->m_size += (freed_block->m_next->m_size + sizeof(t_block));

        printf("Merging block %p with next one %p because both are free\nBlock %p is now size %zu\n",
        (void*) freed_block, (void*) freed_block->m_next, (void*) freed_block, freed_block->m_size);

        freed_block->m_next->m_free = false;
        if (NULL!= freed_block->m_next->m_next)
        {
            t_block*    temp_next = freed_block->m_next->m_next;
            freed_block->m_next = temp_next;
            temp_next->m_prev = freed_block;
        }
        else
            freed_block->m_next->m_free = false;
    }
    if (NULL != freed_block->m_prev && freed_block->m_prev->m_free)
    {
        freed_block->m_prev->m_size += (freed_block->m_size + sizeof(t_block));
        printf("Merging block %p with previous one %p because both are free\nBlock %p is now size %zu\n",
        (void*) freed_block, (void*) freed_block->m_prev,(void*) freed_block->m_prev, freed_block->m_prev->m_size);
        if (NULL != freed_block->m_next)
        {
            freed_block->m_next->m_prev = freed_block->m_prev;
            freed_block->m_prev->m_next = freed_block->m_next;
        }
        freed_block->m_free = false;
    }
}
