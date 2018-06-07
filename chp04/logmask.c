#include <syslog.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /**
     * #include <syslog.h>
     * void openlog(const char *ident, int logopt, int facility);
     * 可以通过openlog来改变日志信息的表示方式.它可以设置一个字符串ident,该字符
     * 串会添加到日志信息的前面.可以通过它来指明是哪个程序创建了这条信息.facility
     * 参数记录设施值.logopt参数对syslog调用行为进行配置.它是如下参数的按位或:
     * LOG_PID: 日志信息中包含进程标识符.
     * LOG_CONS: 如果信息不能被记录到日志文件中,就将其发送到控制台.
     * LOG_ODELAY: 第一次调用syslog时才打开日志设施.
     * LOG_NDELAY: 立即打开日志设施,而不是等到第一次记录日志时.
     */
    int logmask;
    openlog("logmask", LOG_PID | LOG_CONS, LOG_USER);
    // May 17 00:56:57 hill-Inspiron-5439 logmask[21865]: informative message, pid = 21865
    // May 17 00:56:57 hill-Inspiron-5439 logmask[21865]: debug message, should appear
    syslog(LOG_INFO, "informative message, pid = %d", getpid());
    syslog(LOG_DEBUG, "debug message, should appear");
    /**
     * 可以使用setlogmask函数来设置一个日志掩码,并通过它来控制日志信息的优先级.
     * 优先级未在日志掩码中置位的后续syslog调用都将被丢弃.可以通过调用这个方法关
     * 闭LOG_DEBUG消息而不用改变程序主体.
     * 可以用LOG_MASK(priority)为日志信息创建一个只包含一个优先级的掩码,
     * 还可以用LOG_UPTO(priority)来创建一个由指定优先级之上的所有优先级
     * (包括指定优先级)构成的掩码.
     */
    logmask = setlogmask(LOG_UPTO(LOG_NOTICE));
    syslog(LOG_DEBUG, "debug message, should not appear");
    return 0;
}
