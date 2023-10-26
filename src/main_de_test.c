#include <stdio.h> 

#include "malloc_perso.h"
#include "free_perso.h"
#include "calloc_realloc_perso.h"

#include <limits.h> // test type limit values in malloc


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
  

  printf("\n----------------------------\nSTEP BONUS: REALLOC\n");
  float* ptr_to_float = (float*) malloc_perso(sizeof(float) + 5);
  *ptr_to_float = 7.5;

  printf("Before realloc :%f\n\n", *ptr_to_float);
  float* new_float = realloc_perso(ptr_to_float, sizeof(float));
  printf("After realloc: %f\n\n", *new_float);

  printf("Filling free space:\n");
  char* new_string = (char*) malloc_perso(9648);
  printf("\n");
  printf("Realloc (bigger)\n");
  float* float2 = realloc_perso(new_float, 35);

  printf("\n----------------------------\nSTEP BONUS: CALLOC\n");
  int* test_calloc = calloc_perso(4, sizeof(int));

  printf("Before init :\n");
  for (int index = 0; index < 4; index++)
  printf("%p\t%d\n", (void*) (test_calloc + index), test_calloc[index]);

  *test_calloc = 1;
  *(test_calloc + 1) = 2;
  *(test_calloc + 2) = 3;
  *(test_calloc + 3) = 4;

  printf("After init :\n");
  for (int index = 0; index < 4; index++)
  printf("%p\t%d\n", (void*) (test_calloc + index), test_calloc[index]);
  
  printf("\n----------------------------\nEDGE VALUE TESTS: MALLOC & FREE\n");
  printf("\nsize = -1 (should be null):\n");
  void* negative_test = malloc_perso(-1);
  if (NULL == negative_test)
  printf("is null\n");

  printf("\nsize = 0 (should be null):\n");
  void* zero_test = malloc_perso(0);
  if (NULL == zero_test)
  printf("is null\n");

  printf("\nsize = 1 MiB (should extend heap):\n");
  void* mib_test = malloc_perso(1048576);
  if (NULL == mib_test)
  printf("is null\n");

  printf("\nsize = ULONG_MAX - 48 (should extend heap and be biggest allocation possible):\n");
  void* ulong_test3 = malloc_perso(4294967295 - 48);
  if (NULL == ulong_test3)
  printf("is null\n\n");

  printf("\nsize = ULONG_MAX - 16 (should be null):\n");
  void* ulong_test1 = malloc_perso(4294967295 - 16);
  if (NULL == ulong_test1)
  printf("is null\n\n");


  printf("Do not defrag if block is too big:\n");
  free_perso(mib_test);
  free_perso(ulong_test3);
  printf("sizeof mib_test : %lu (should be 1048576)\n", ((t_block*)(mib_test)-1)->m_size);

  printf("\nsize = ULONG_MAX (should be null):\n");
  void* ulong_test2 = malloc_perso(ULONG_MAX);
  if (NULL == ulong_test2)
  printf("is null\n");

  printf("\nsize = ULONG_MAX - 47 (should be null):\n");
  void* ulong_test4 = malloc_perso(ULONG_MAX - 47);
  if (NULL == ulong_test4)
  printf("is null\n");

  printf("\nsize = ULLONG_MAX (should be null):\n");
  void* ullong_test = malloc_perso(ULLONG_MAX);
  if (NULL == ullong_test)
  printf("is null\n");

  printf("\nsize = INT_MIN (should be null):\n");
  void* int_test = malloc_perso(INT_MIN);
  if (NULL == int_test)
  printf("is null\n");

  printf("\nsize = LLONG_MIN (should be null):\n");
  void* llong_test = malloc_perso(LLONG_MIN);
  if (NULL == llong_test)
  printf("is null\n\n");

  printf("Freeing valid pointers:\n\n");
  printf("sizeof float2 : %lu\n", ((t_block*)(float2)-1)->m_size);
  free_perso(float2);
  printf("sizeof test_calloc: %lu\n", ((t_block*)(test_calloc)-1)->m_size);
  free_perso(test_calloc);
  printf("sizeof new_string : %lu\n", ((t_block*)(new_string)-1)->m_size);
  free_perso(new_string);

  printf("\nFreeing null pointers:\n\n");
  free_perso(negative_test);
  free_perso(zero_test);
  free_perso(ulong_test1);
  free_perso(ulong_test2);
  free_perso(ulong_test4);
  free_perso(ullong_test);
  free_perso(int_test);
  free_perso(llong_test);

 
  return 0;
}
