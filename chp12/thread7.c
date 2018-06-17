#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

int main() {
    int res;
    pthread_t a_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    sleep(3);
    
    /**
     * int pthread_cancel(pthread_t thread);
     * 向thread发送取消请求.
     */
    
    res = pthread_cancel(a_thread);
    printf("Canceling thread...\n");
    if (res != 0) {
        perror("Thread cancelation failed");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < 10; i++) {
        printf("test is still running (%d)...\n", i);
        sleep(1);
    }
    sleep(4);
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    int i, res, j;
    /**
     * int pthread_setcancelstate(int state, int *oldstate);
     * state的值可以为PTHREAD_CANCEL_ENABLE允许线程接收取消请求;
     * PTHREAD_CANCEL_DISABLE忽略取消请求.
     * oldstate指针用于获取先前的取消状态.
     */
    res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (res != 0) {
        perror("Thread pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }
    /**
     * int pthread_setcanceltype(int type, int *oldtype);
     * type取值PTHREAD_CANCEL_ASYNCHRONOUS,接收到取消请求后立即采取行动;
     * PTHREAD_CANCEL_DEFERRED,接收到取消请求后,一直等待直到线程执行了下述
     * 函数之一后才采取行动:pthread_join,pthread_cond_wait,
     * pthread_cond_timedwait,pthread_testcancel,sem_wait,sigwait.
     */
    res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    if (res != 0) {
        perror("Thread pthread_setcanceltype failed");
        exit(EXIT_FAILURE);
    }
    printf("thread_function is running\n");
    for(i = 0; i < 10; i++) {
        printf("Thread is still running (%d)...\n", i);
        sleep(1);
    }
    pthread_exit(0);
}
