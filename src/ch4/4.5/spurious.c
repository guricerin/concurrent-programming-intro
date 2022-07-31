#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// シグナルハンドラ
void handler(int sig)
{
    printf("received signal: %d\n", sig);
}

// 疑似覚醒を発生させるプログラム
int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf("pid: %d\n", pid);

    // シグナルハンドラ登録
    signal(SIGUSR1, handler);

    // waitしているが、誰もnotifyしないので止まったままのはず。
    // ただし、OpenBSDやmacなどで、別のコンソールからシグキルを送信するとプログラムが終了する。
    pthread_mutex_lock(&mutex);
    if (pthread_cond_wait(&cond, &mutex) != 0)
    {
        perror("pthread_cond_wait");
        exit(1);
    }
    printf("spurious wake up\n");
    pthread_mutex_unlock(&mutex);

    return 0;
}
