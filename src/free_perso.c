#include "free_perso.h"
#include "malloc_perso.h"
#include <unistd.h>
#include <stdio.h>

void free_perso (void *ptr)
{
    if (NULL != ptr)
    {
        t_block*    list_member = (t_block*) g_head;

        while (NULL != list_member)
        {
            if (list_member + 1 == ptr && !list_member->m_free)
            {
                printf("Freeing block %p\n", (void*) (list_member));
                list_member->m_free = true;
                try_to_fusion(list_member);
                ptr = NULL;
                return;
            }
            list_member = list_member->m_next;
        }
    } 
    printf("Invalid pointer.\n");
}