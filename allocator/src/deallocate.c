#include <unistd.h>
#include <stdio.h>

#include "allocate.h"

void deallocate (void *ptr)
{
    // Do not free anything if pointer is null
    if (NULL != ptr)
    {
        // Temp variable to parse block linked list
        t_block*    list_member = (t_block*) g_head;

        // Stop when finished parsing list
        while (NULL != list_member)
        {
            // If list_member + 32 bytes is equal to ptr argument, free it
            if (list_member + 1 == ptr && !list_member->m_free)
            {
                // Displat successful free message
                printf("Freeing block %p\n", (void*) (list_member));

                // Set m_free to true so block can be reused when calling allocate
                list_member->m_free = true;

                // Try to defragment block with next and previous blocks
                defrag(list_member);
                
                // Set pointer to NULL since block was freed
                ptr = NULL;
                return;
            }
            list_member = list_member->m_next;
        }
    } 

    // If pointer is null or not in linked list, display failure message
    printf("Invalid pointer.\n");
}