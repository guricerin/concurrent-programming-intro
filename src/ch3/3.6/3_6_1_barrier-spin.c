#include <pthread.h>
#include <stdio.h>

#define NUM_THREAD 10

void barrier(volatile int *cnt, int max)
{
    __sync_fetch_and_add(cnt, 1);
    while (*cnt < max)
        ;
}

volatile int num = 0; // 共有変数

void *worker(void *arg) // スレッド用関数
{
    barrier(&num, NUM_THREAD); // 全スレッドがここまで到達するまで待機

    {
        // 何らかの処理
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // スレッド生成
    pthread_t th[NUM_THREAD];
    for (int i = 0; i < NUM_THREAD; i++)
    {
        if (pthread_create(&th[i], NULL, worker, NULL) != 0)
        {
            perror("pthread_create");
            return -1;
        }
    }

    // joinは省略
    return 0;
}
