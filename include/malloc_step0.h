#ifndef MALLOC_STEP0_H
#define MALLOC_STEP0_H

#define ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

void* throwaway_malloc(size_t size);

void throwaway_free(void* to_free);

#endif


