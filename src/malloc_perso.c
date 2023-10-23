#include "malloc_perso.h"
#include <unistd.h>

t_block* g_head = NULL;

void* malloc_perso(size_t size)
{
    t_block*    found_block = find_block(size);

    if (found_block != NULL)
        return (void*) ++found_block;

    found_block = extend_heap(size);
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

// void initialize_block(t_block* block)
// {

// }

t_block* find_block(size_t size)
{
    t_block*    list_member = g_head;

    size = ALIGN_UP(size, 16);
    while (NULL != list_member)
    {
        if (list_member->m_free && list_member->m_size > size)
            return split_block(list_member, size);
        else if (list_member->m_free && list_member->m_size == size)
        {
            // printf("fit found\n");
            // printf("pointer to metadata: %p\n", (void*)list_member);
            list_member->m_free = false;
            return list_member;
        }
        list_member = list_member->m_next;
    }
    return NULL;
}

t_block* split_block (t_block* b, size_t size)
{
    t_block*      remainder_block = b + 2;

    remainder_block->m_prev = b;
    remainder_block->m_next = b->m_next;
    remainder_block->m_size = b->m_size - size;
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
        freed_block->m_size += freed_block->m_next->m_size;
        if (NULL!= freed_block->m_next->m_next)
        {
            t_block*    temp_next = freed_block->m_next->m_next;

            freed_block->m_next = (void*) freed_block->m_next;
            freed_block->m_next = temp_next;
            temp_next->m_prev = freed_block;
        }
        else
            freed_block->m_next = (void*) freed_block->m_next;
        //printf("new size : %zu\n", freed_block->m_size);
        //printf("Succesfully defragmented\n");
        
    }
    if (NULL != freed_block->m_prev && freed_block->m_prev->m_free)
    {
        freed_block->m_prev->m_size += freed_block->m_size;
        if (NULL != freed_block->m_next)
        {
            freed_block->m_next->m_prev = freed_block->m_prev;
            freed_block->m_prev->m_next = freed_block->m_next;
        }
        //printf("new size : %zu\n", freed_block->m_prev->m_size);
        freed_block = (void*) freed_block;
    //printf("Succesfully defragmented\n");
    }
}