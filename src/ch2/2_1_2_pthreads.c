#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Pthreadsではスレッド用関数の型は、void*型の値を受け取り、void*型の値をリターンする必要がある。
void *thread_func(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        printf("i = %d\n", i);
        sleep(1);
    }

    return "finished!";
}

int main(int argc, char *argv[])
{
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0)
    {
        perror("pthread_attr_init");
        return -1;
    }
    // デタッチスレッドに設定
    // デタッチスレッド : スレッド終了時にスレッド用のリソースを自動的に破棄
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        perror("pthread_attr_setdetachstate");
        return -1;
    }

    pthread_t th;
    // スレッド生成
    if (pthread_create(&th, &attr, thread_func, NULL) != 0)
    {
        perror("pthread_create");
        return -1;
    }

    // アトリビュート破棄
    if (pthread_attr_destroy(&attr) != 0)
    {
        perror("pthread_attr_destroy");
        return -1;
    }

    sleep(7);

    return 0;
}
