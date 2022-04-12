#include "../3.2/3_2_2_tas.c"

void spinlock_acquire(bool *lock)
{
    while (test_and_set(lock)) // TASを用いてロックを獲得できるまでループ（ポーリング方式）
        ;
    // クリティカルセクションはこの関数の呼び出し元
}

void spinlock_release(bool *lock)
{
    tas_release(lock);
}
