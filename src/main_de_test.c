
#include <stdio.h> // printf

#include "malloc_perso.h"
#include "free_perso.h"


int main()
{
  printf("----------------------------\nSTEP 1: First malloc: no bloc SHOULD EXTEND HEAP\n\n");
  int* test = (int*)malloc_perso(4096);
  printf("\nCHECK: Pointer %p is aligned on 4 ? %s\n",
	 (void*)test, ((size_t)test % 4) == 0 ? "yes" : "no" );
  
  test[0] = 1;
  test[1] = 2;
  test[2] = 2;

  printf("\n----------------------------\nSTEP 2 : First free\n\n");
  free_perso(test);

  printf("\n----------------------------\nSTEP 3: Second malloc fitting bloc SHOULD SPLIT\n\n");
  int* test2 = (int*)malloc_perso(16);  
  test2[0] = 2323;

  printf("\nCHECK: Pointer test2 should be equal to test (reuse first block): %p = %p\n",
	 (void*)test, (void*)test2);
  

  printf("\n----------------------------\nSTEP 4: third malloc should take new split block\n");
  int* test3 = (int*)malloc_perso(4030);
  test3[0] = 111111;

  printf("\nCHECK: Pointer %p is aligned on 4 ? %s\n",
	 (void*)test3, ((size_t)test3 % 4) == 0 ? "yes" : "no" );


  printf("\n----------------------------\nSTEP 5: Second free should free first block\n\n");
  free_perso(test2);
  
  
  printf("\n----------------------------\nSTEP 6: Fourth malloc should take first block without split\n\n");
  int* test4 = (int*)malloc_perso(12);
  test4[1] = 3;

  printf("\nCHECK: Pointer test4 should be equal to test (reuse first block): %p = %p\n",
	 (void*)test, (void*)test4);

  
  printf("\n----------------------------\nSTEP 7: Fifth malloc no available bloc - SHOULD EXTEND HEAP\n\n");
  int* test5 = (int*)malloc_perso(5555);
  test5[33] = 3;

  printf("\nCHECK: Pointer test5 should be a new block (no block reuse): %s\n",
	 (test5 != test && test5 != test3) ? "OK" : "FAIL");

  printf("\nCHECK: Pointer %p is aligned on 4 ? %s\n",
	 (void*)test5, ((size_t)test5 % 4) == 0 ? "yes" : "no" );
    
  printf("\n----------------------------\nSTEP 8: Free the two first blocks - SHOULD FUSION\n");
  printf("\nFirst free\n\n"); 
  free_perso(test3);
  printf("\nSecond free\n\n"); 
  free_perso(test4);


  printf("\n----------------------------\nSTEP 9: Final free - should merge\n");
  free_perso(test5);
  
  return 0;
}
