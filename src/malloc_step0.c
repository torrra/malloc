#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "malloc_step0.h"
#include "malloc_perso.h"

// #ifndef NULL
// #define NULL ((void *)0)
// #endif

#include <stdio.h>

t_block*     g_head = NULL;

int main (void)
{   
    int* malloc1 = (int*) throwaway_malloc(sizeof(int));
    *malloc1 = 4;
    printf("Pointer to data: %p\ndata: %d\n\n", (void*) malloc1, *malloc1);

    void* malloc2 = throwaway_malloc(8);
    printf("Pointer to data: %p\n\n", malloc2);
    void* malloc3 = throwaway_malloc(26);
    printf("Pointer to data: %p\n\n", malloc3);
    void* malloc4 = throwaway_malloc(2);

    printf("Pointer to data: %p\n\n", malloc4);
    printf("Pointer to head metadata: %p\n", (void*) g_head);
    printf("sizeof struct : %zu\n", sizeof(t_block));

    throwaway_free(malloc1);
    throwaway_free(malloc2);
    throwaway_free(malloc3);
    throwaway_free(malloc4);
    throwaway_free(NULL);

}

void* throwaway_malloc(size_t size)
{   
    t_block*    list_member = g_head;

    size = ALIGN_UP(size, 16);
    while (NULL != list_member)
    {
        if (list_member->m_free && list_member->m_size > size)
            return fragment_block(list_member, size);
        else if (list_member->m_free && list_member->m_size == size)
        {
            list_member->m_free = false;
            return (void*) ++list_member;
        }
        list_member = list_member->m_next;
    }
    if (sbrk(size + sizeof(t_block) + sizeof(t_footer)) == (void*) -1)
        return NULL;

    t_block*    new_block = (t_block*) sbrk(0);

    new_block->m_size = size;
    //t_footer*   new_footer = new_block + (sizeof(t_block) + size);

    if (NULL == g_head)
    {
        g_head = new_block;
        g_head->m_prev = NULL;
        g_head->m_next = NULL;
    }
    else
    {
        t_block*    list_member = g_head;

        while (NULL != list_member->m_next)
            list_member = list_member->m_next;

        list_member->m_next = new_block;
        new_block->m_prev = list_member;
    }

    //new_footer->m_data_size = size;
    printf("Total allocated space: %zu\nPointer to metadata: %p\n", sizeof(t_block) + size, (void*) new_block);
    new_block->m_next = NULL;
    return (void*) ++new_block; // Return pointer to data only
}

void throwaway_free(void* to_free)
{
    if (NULL != to_free)
    {
        t_block*    list_member = g_head;
    //bool        found = false;
    
        while (NULL != list_member)
        {
            if (list_member + 1 == to_free)
            {
                t_block*    cast_data = (t_block*) to_free;

                cast_data--;
                cast_data->m_free = true;
                return;
            }
            list_member = list_member->m_next;
        }
    } 
    //perror("Invalid pointer."); 
    printf("Invalid pointer.\n");
}

t_block* fragment_block(t_block* big_block, size_t to_alloc)
{
    t_block*      rest_block = big_block + sizeof(t_block) + to_alloc;

    rest_block->m_prev = big_block;
    rest_block->m_next = big_block->m_next;
    rest_block->m_size = big_block->m_size - to_alloc; 

    big_block->m_next = rest_block;
    big_block->m_size = to_alloc;
    big_block->m_free = false;

    return (void*) ++big_block;
}