#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sigset_t set;

// シグナル受信用のスレッド
void *handler(void *arg)
{
    pthread_detach(pthread_self()); // デタッチしているが、必須ではない。

    int sig;
    for (;;)
    {
        if (sigwait(&set, &sig) != 0) // シグナルを受信
        {
            perror("sigwait");
            exit(1);
        }
        printf("received signal: %d\n", sig);
        pthread_mutex_lock(&mutex);
        // 何らかの処理
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *worker(void *arg)
{
    for (int i = 0; i < 10; i++)
    {
        printf("worker cnt: %d\n", i);
        pthread_mutex_lock(&mutex);
        // 何らかの処理
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf("pid: %d\n", pid);

    // SIGUSR1シグナルをブロックに設定。
    // この設定は、後に作成されるスレッドにも引き継がれる。
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    // シグナルブロック : ブロックされたシグナルがプロセスに送信されても、シグナルハンドラが起動されなくなる。
    // -> handler()でのロック獲得が行われなくなり、デッドロックが発生しない。
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
    {
        perror("pthread_sig_mask");
        return 1;
    }

    pthread_t th, wth;
    pthread_create(&th, NULL, handler, NULL);
    pthread_create(&wth, NULL, worker, NULL);
    pthread_join(wth, NULL);

    return 0;
}
