#include "../3.2/3_2_2_tas.c"

void spinlock_acquire(volatile bool *lock) // ループ中の最適化を防ぐため、volatileとしている。
{
    for (;;)
    {
        while (*lock) // アトミック命令を無駄に呼び出す回数を減らす。
            ;

        if (!test_and_set(lock))
            break;
    }
}

void spinlock_release(bool *lock)
{
    tas_release(lock);
}
