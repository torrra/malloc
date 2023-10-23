#include "free_perso.h"
#include "malloc_perso.h"
#include <unistd.h>
#include <stdio.h>

void free_perso (void *ptr)
{
    if (NULL != ptr)
    {
        t_block*    list_member = (void*) g_head;

        while (NULL != list_member)
        {
            if (list_member + 1 == ptr)
            {
                t_block*    cast_data = (t_block*) ptr;

                cast_data--;
                printf("Freeing block %p\n", (void*) (cast_data));
                cast_data->m_free = true;
                try_to_fusion(cast_data);
                ptr = NULL;
                return;
            }
            list_member = list_member->m_next;
        }
    } 
    printf("Invalid pointer.\n");
}