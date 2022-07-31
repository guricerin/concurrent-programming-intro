#include "../../../ch3/3.3/3_3_1_spinlock-fast.c"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/// 再入可能ミューテックス用の型
struct reent_lock
{
    bool lock; // ロック用共有変数
    int id;    // 現在ロックを獲得中のスレッドID。0の場合は誰もロック獲得していない。
    int cnt;   // 再帰ロックを何回行ったか
};

// 再帰ロック獲得関数
void reentlock_acquire(struct reent_lock *lock, int id)
{
    // ロック獲得中でかつ自分が獲得中か判定
    if (lock->lock && lock->id == id)
    {
        lock->cnt++;
    }
    else
    {
        // 誰もロックを獲得していないか、他のスレッドがロック獲得中ならロック獲得
        spinlock_acquire(&lock->lock);
        // ロックを獲得したら自身のスレッドIDを設定し、再帰カウントをインクリメント
        __atomic_store_n(&lock->id, id, __ATOMIC_RELAXED);
        lock->cnt++;
    }
}

// 再帰ロック解放関数
void reentlock_release(struct reent_lock *lock)
{
    lock->cnt--;
    if (lock->cnt == 0)
    {
        // カウントが0になってはじめて、ロックを解放
        __atomic_store_n(&lock->id, 0, __ATOMIC_RELAXED);
        spinlock_release(&lock->lock);
    }
}

struct reent_lock lock_var;

void reent_lock_test(int id, int n)
{
    if (n == 0)
        return;

    // 再帰ロック
    reentlock_acquire(&lock_var, id);
    reent_lock_test(id, n - 1);
    reentlock_release(&lock_var);
}

void *thread_func(void *arg)
{
    int id = (int)arg;
    assert(id != 0);
    for (int i = 0; i < 10000; i++)
    {
        reent_lock_test(id, 10);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t v[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&v[i], NULL, thread_func, (void *)(i + 1));
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(v[i], NULL);
    }
    return 0;
}
