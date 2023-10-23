#include <stdio.h>
#include "malloc_perso.h"
#include "free_perso.h"

 int main (void)
{   
    int* malloc1 = (int*) malloc_perso(sizeof(int));
    *malloc1 = 4;
    //printf("Pointer to data: %p\ndata: %d\n\n", (void*) malloc1, *malloc1);

    void* malloc2 = malloc_perso(8);
    //printf("Pointer to data: %p\n\n", malloc2);

    free_perso(malloc2);
    void* malloc3 = malloc_perso(26);
    //printf("Pointer to data: %p\n\n", malloc3);

    void* malloc4 = malloc_perso(5);
    //printf("Pointer to data: %p\n\n", malloc4);
    
    free_perso(malloc1);
    free_perso(malloc3);
    free_perso(malloc4);
    void* malloc5 = malloc_perso(2);
    //printf("Pointer to data: %p\n\n", malloc5);

    int* malloc6 = (int*) malloc_perso(18);
    //printf("Pointer to data: %p\n\n", (void*)malloc6);
    *malloc6 = 2;

    //printf("Pointer to head metadata: %p\n", (void*) g_head);
    //printf("sizeof struct : %zu\n", sizeof(t_block)); 
   
    free_perso(malloc5);
    free_perso(NULL);
    free_perso(malloc6);

}