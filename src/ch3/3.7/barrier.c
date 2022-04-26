#include <stdlib.h>

void barrier(volatile int *cnt, int max)
{
    __sync_fetch_and_add(cnt, 1);
    while (*cnt < max)
        ;
}
