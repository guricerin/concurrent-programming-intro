#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t barrier_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;

#define NUM_THREAD 10

void barrier(volatile int *cnt, int max)
{
    if (pthread_mutex_lock(&barrier_mut) != 0)
    {
        perror("pthread_mutex_lock");
        exit(-1);
    }

    (*cnt)++;

    if (*cnt == max)
    {
        // 全プロセスが揃ったので通知
        if (pthread_cond_broadcast(&barrier_cond) != 0)
        {
            perror("pthread_cond_broadcast");
            exit(-1);
        }
    }
    else
    {
        // 全プロセスが揃うまで待機
        do
        {
            if (pthread_cond_wait(&barrier_cond, &barrier_mut) != 0)
            {
                perror("pthread_cond_wait");
                exit(-1);
            }
        } while (*cnt < max) // 疑似覚醒のための条件
            ;
    }

    if (pthread_mutex_unlock(&barrier_mut) != 0)
    {
        perror("pthread_mutex_unlock");
        exit(-1);
    }
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
