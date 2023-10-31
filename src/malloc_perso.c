#include <unistd.h>
#include <stdio.h>

#include "malloc_perso.h"

void*   g_head = NULL;

void* malloc_perso(size_t size)
{    
    /* Check if size taken from user is 0 or too large (negative numbers are turned into
    largest possible value of size_t, therefore too large) */
    if (0 == size || size >= MAX_SIZE_T - sizeof(t_block) || 
    size + sizeof(t_block) > MAX_SIZE_T - ALIGNMENT)
        return NULL;

    // Store unaligned size to display it with printf
    size_t      temp_size = size;

    // Align size on 16
    size = ALIGN_UP(size, ALIGNMENT);

    // Check if a free block already exists
    t_block*    found_block = find_block(size);

    // If valid block has been found, return data address (found_block + 32 bytes)
    if (found_block != NULL)
        return (void*) ++found_block;

    // If not, create new block
    found_block = extend_heap(size);

    // Visually check if block and data addresses are correct
    printf("I'm going to alloc %zu bytes (sizeof t_block: %zu  + requested size %zu (aligment 16))\
\nThe pointer to block is %p  / block end is %p / so aligned data block is at %p\n\
I found no free block: created a new one %p\n",
    size + sizeof(t_block), sizeof(t_block), temp_size, (void*) found_block, (void*) (found_block + 1), 
    (void*) (found_block + 1), (void*) found_block);
    
    // Return data address
    return (void*) ++found_block;
}

t_block* extend_heap(size_t size)
{
    // Check if heap extension has failed
    if (sbrk(size + sizeof(t_block)) == (void*) -1)
        return NULL;

    //If successful, store adress of new block
    t_block*    new_block = (t_block*) sbrk(0);

    // Initialize block
    new_block -> m_size = size;
    new_block -> m_next = NULL;
    new_block -> m_free = false;

    // If it is the first block ever created, g_head should point to it
    if (NULL == g_head)
    {
        g_head = new_block;
        new_block -> m_prev = NULL;
    }
    
    // If not, parse block list and link new block at the end
    else
    {
        // Temp variable to parse list and store last block in linked list
        t_block*    list_member = g_head;

        while (NULL != list_member -> m_next)
            list_member = list_member -> m_next;

        list_member -> m_next = new_block;
        new_block -> m_prev = list_member; 
    }
    return new_block;
}

void initialize_block(t_block* block)
{
    // Do not initialize anything if pointer is NULL
    if (NULL == block)
        return;

    block -> m_size = 0;
    block -> m_next = NULL;
    block -> m_prev = NULL;
    block -> m_free = true;
}

t_block* find_block(size_t size)
{
    // Temp variable to parse list
    t_block*    list_member = (t_block*) g_head;

    // Stop  when finished parsing list
    while (NULL != list_member)
    {
        // If a free block is large enough to hold data + new t_block + minimum data size (16), split and return
        if (list_member -> m_free && list_member -> m_size > size + sizeof(t_block) + ALIGNMENT)
        {
            printf("I found free block %p\n", (void*) list_member);
            return split_block(list_member, size);
        }

        // If a free block is large enough to hold data but no new block only return, only return
        else if (list_member -> m_free && list_member -> m_size >= size)
        {
            printf("I found free block %p\n", (void*) list_member);
            list_member -> m_free = false;
            return list_member;
        }

        // Go to next block if current one is either not free or too small
        list_member = list_member -> m_next;
    }

    // Return NULL if no suitable block was found
    return NULL;
}

t_block* split_block (t_block* b, size_t size)
{
    // Calculate size and adress of leftover block after split
    t_block*      remainder_block = (t_block*) ((__intptr_t) b + (sizeof(t_block) + size));
    size_t        remainder_size = b -> m_size - (size + sizeof(t_block));

    // Display block that will be split along with remainder_block (with their respective sizes)
    printf("Block %p of size %zu can be split when allocating %zu bytes because block size > \
mimimumSplitBlockSize %zu\nSplitting block %p to make it size %zu and creating \
new block %p having size %zu\n",
    (void*) b, b -> m_size, size, size + sizeof(t_block), (void*) b, size, 
    (void*) remainder_block, remainder_size);

    // Initialize new block 
    remainder_block -> m_prev = b;
    remainder_block -> m_next = b -> m_next;
    remainder_block -> m_size = remainder_size;
    remainder_block -> m_free = true;

    // Reassign relevant values to block to return
    b -> m_next = remainder_block;
    b -> m_size = size;
    b -> m_free = false;

    return b;
}

void try_to_fusion(t_block* freed_block)
{
    /* If next block is free and small enough for its size to fit in a size_t variable 
    when fused with current block, defragment */
    if (NULL != freed_block -> m_next && freed_block -> m_next -> m_free &&
    (large_size_t) (freed_block -> m_size + freed_block -> m_next -> m_size + sizeof(t_block)) < MAX_SIZE_T)
    {
        // Add size next block size (+ the size of its metadata t_block) to current block
        freed_block -> m_size += (freed_block -> m_next -> m_size + sizeof(t_block));

        // Show what blocks will me merged
        printf("Merging block %p with next one %p because both are free\nBlock %p is now size %zu\n",
        (void*) freed_block, (void*) freed_block -> m_next, 
        (void*) freed_block, freed_block -> m_size);

        // Set m_free boolean to false so it is not reused when calling malloc_perso
        freed_block -> m_next -> m_free = false;

        // If there is a block after the next one, make it the current block's next block
        if (NULL!= freed_block -> m_next -> m_next)
        {
            freed_block -> m_next = freed_block -> m_next -> m_next;
            freed_block -> m_next -> m_prev = freed_block;
        }
    }
    /* If previous block is free and its size is smalle enough to fit in a size_t variable
    when fused with current block, defragment */
    if (NULL != freed_block -> m_prev && freed_block -> m_prev -> m_free &&
    (large_size_t) (freed_block -> m_prev -> m_size + freed_block -> m_size + sizeof(t_block)) < MAX_SIZE_T)
    {
        // Add current block size to previous block
        freed_block -> m_prev -> m_size += (freed_block -> m_size + sizeof(t_block));

        // Show what blocks will be merged
        printf("Merging block %p with previous one %p because both are free\nBlock %p is now size %zu\n",
        (void*) freed_block, (void*) freed_block -> m_prev,
        (void*) freed_block -> m_prev, freed_block -> m_prev -> m_size); 

        // Set m_free boolean to false so it is not reused
        freed_block->m_free = false;

        // If there is a block after the current block, make it the previous block's next block
        if (NULL != freed_block -> m_next)
        {
            freed_block -> m_next -> m_prev = freed_block -> m_prev;
            freed_block -> m_prev -> m_next = freed_block -> m_next;
        }   
    }
}