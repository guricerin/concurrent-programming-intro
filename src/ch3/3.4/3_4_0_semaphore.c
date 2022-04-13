#define NUM 4

void semaphore_acquire(volatile int *cnt) // ロックを獲得しているプロセス数を把握するため、int型の共有変数となっている。
{
    for (;;)
    {
        while (*cnt >= NUM) // 待機
            ;
        __sync_fetch_and_add(cnt, 1);
        if (*cnt <= NUM) // ループを抜けてロックを獲得。
        {
            break;
        }
        __sync_fetch_and_sub(cnt, 1); // 複数のプロセスが同時にロックを獲得したため、リトライ。
    }
}

void semaphore_release(int *cnt)
{
    __sync_fetch_and_sub(cnt, 1);
}

#include "semtest.c"
