#include "../3.3/3_3_1_spinlock-fast.c"

// Reader用ロック獲得関数
void rwlock_read_acquire(int *rcnt, volatile int *wcnt)
{
    for (;;)
    {
        while (*wcnt) // Writerがいるならスピンして待機
            ;
        __sync_fetch_and_add(rcnt, 1);
        if (*wcnt == 0) // Writerがいない場合にロック獲得。再度wcntをチェックしているのは、rcntのインクリメント中にwcntがインクリメントされている可能性があるため。
            break;
        __sync_fetch_and_sub(rcnt, 1);
    }
}

// Reader用ロック解放関数
void rwlock_read_release(int *rcnt)
{
    __sync_fetch_and_sub(rcnt, 1);
}

// Writer用ロック獲得関数
void rwlock_write_acquire(bool *lock, volatile int *rcnt, int *wcnt)
{
    __sync_fetch_and_add(wcnt, 1);
    while (*rcnt) // Readerがいるなら待機
        ;
    spinlock_acquire(lock);
}

// Writer用ロック解放関数
void rwlock_write_release(bool *lock, int *wcnt)
{
    spinlock_release(lock);
    __sync_fetch_and_sub(wcnt, 1);
}

// 共有変数
int rcnt = 0;
int wcnt = 0;
bool lock = false;

void reader()
{
    for (;;)
    {
        rwlock_read_acquire(&rcnt, &wcnt);
        {
            // クリティカルセクション（読み込みのみ）
        }
        rwlock_read_release(&rcnt);
    }
}

void writer()
{
    for (;;)
    {
        rwlock_write_acquire(&lock, &rcnt, &wcnt);
        {
            // クリティカルセクション（読み書き）
        }
        rwlock_write_release(&lock, &rcnt);
    }
}
