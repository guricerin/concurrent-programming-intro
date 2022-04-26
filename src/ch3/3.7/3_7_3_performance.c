#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// do_lock関数の切り替え
#ifdef RWLOCK
#include "rwlock.c"
#elif defined(RWLOCK_WR)
#include "rwlock-wr.c"
#elif defined(MUTEX)
#include "mutex.c"
#elif defined(EMPTY)
#include "empty.c"
#endif

#include "barrier.c"

volatile int flag = 0; // このフラグが0の間はループ

// バリア同期用変数
volatile int waiting_1 = 0;
volatile int waiting_2 = 0;

// ワーカスレッドが最終的に実行できたクリティカルセクションの回数を記録。
uint64_t count[NUM_THREAD - 1];

void *worker(void *arg)
{
    uint64_t id = (uint64_t)arg;
    printf("thread id: %lu\n", id);
    barrier(&waiting_1, NUM_THREAD); // バリア同期

    uint64_t n = 0; // 実行できたクリティカルセクションの回数
    while (flag == 0)
    {
        do_lock(); // 必要ならロックを獲得して待機
        n++;
    }
    count[id] = n; // 何回ループしたかを記録

    barrier(&waiting_2, NUM_THREAD); // バリア同期

    return NULL;
}

// タイマスレッド用関数
void *timer(void *arg)
{
    barrier(&waiting_1, NUM_THREAD);

    sleep(100);
    flag = 1;

    barrier(&waiting_2, NUM_THREAD); // バリア同期
    for (int i = 0; i < NUM_THREAD - 1; i++)
    {
        printf("%lu\n", count[i]);
    }

    return NULL;
}

int main()
{
    printf("start\n");
    // ワーカスレッド起動
    for (uint64_t i = 0; i < NUM_THREAD - 1; i++)
    {
        pthread_t th;
        pthread_create(&th, NULL, worker, (void *)i);
        pthread_detach(th);
    }

    // タイマスレッド起動
    pthread_t th;
    pthread_create(&th, NULL, timer, NULL);
    pthread_join(th, NULL);

    printf("end\n");
    return 0;
}
