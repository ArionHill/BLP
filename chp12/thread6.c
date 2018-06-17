#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);
char message[] = "Hello World";
int thread_finished = 0;

int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;

    pthread_attr_t thread_attr;

    int max_priority;
    int min_priority;
    struct sched_param scheduling_value;

    /**
     * int pthread_attr_init(pthread_attr_t *attr);
     * 初始化线程属性对象.
     */
    res = pthread_attr_init(&thread_attr);
    if (res != 0) {
        perror("Attribute creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (res != 0) {
        perror("Setting detached attribute failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_attr_setschedpolicy(&thread_attr, SCHED_OTHER);
    if (res != 0) {
        perror("Setting scheduling policy failed");
        exit(EXIT_FAILURE);
    }
    /**
     * int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
     * 第一个参数为指向pthread_t类型数据的指针.线程被创建时,这个指针指向的变量中被写入一个标识符,我们用该标识符来引用新线程.
     * 下一个参数设置线程的属性.不需要特殊属性时设为NULL.
     * 最后两个参数分别告诉线程将要启动执行的函数和传递给该函数的参数.
     * void *(*start_routine)(void *)启动执行的函数以一个指向void的指针为参数,返回的也是一个指向void的指针.
     * 
     * 用fork调用后,父子进程将在同一位置继续执行下去,只是fork调用的返回值是不同的;
     * 但对新线程来说,必须明确地给它提供一个函数指针,新线程将在这个新位置开始执行.
     * 
     * 该函数调用成功时返回0,失败时返回错误代码.
     */
    res = pthread_create(&a_thread, &thread_attr, thread_function, (void *)message);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    max_priority = sched_get_priority_max(SCHED_OTHER);
    min_priority = sched_get_priority_min(SCHED_OTHER);
    scheduling_value.sched_priority = min_priority;
    res = pthread_attr_setschedparam(&thread_attr, &scheduling_value);
    if (res != 0) {
        perror("Setting schedpolicy failed");
        exit(EXIT_FAILURE);
    }

    (void)pthread_attr_destroy(&thread_attr);  // 对属性对象进行清理和回收.一旦对象被回收,除非它被重新初始化,否则就不能被再次使用.
    while (!thread_finished) {
        printf("Waiting for thread to say it's finished...\n");
        sleep(1);
    }
    
    printf("Other thread finished, bye!\n");
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
    printf("thrad_function is running. Argument was %s\n", (char *)arg);
    sleep(3);
    printf("Second thread setting finished flag, and exiting now\n");
    thread_finished = 1;
    pthread_exit(NULL);
}
