#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE *f;

    f = fopen("not_here", "r");
    if (!f) {
        printf("log\n");
        /**
         * #include <syslog.h>
         * void syslog(int priority, const char *message, arguments...);
         * 向系统的日志设施(facility)发送一条日志信息.每条信息都有一个priority参数,
         * 该参数是一个严重级别与一个设施值的按位或.严重级别控制日志信息的处理方式,
         * 设施值记录日志信息的来源.
         * 定义在头文件syslog.h中的设施值包括LOG_USER(默认值)--它指出消息来自一个用
         * 户应用程序,以及LOG_LOCAL0, LOG_LOCAL1直到LOG_LOCAL7,它们的含义由本地管
         * 理员指定.
         * 严重级别按优先级递减排列:
         * 优先级           说明
         * LOG_EMERG       紧急情况
         * LOG_ALERT       高优先级故障, 如数据库崩溃
         * LOG_CRIT        严重错误,如硬件故障
         * LOG_ERR         错误
         * LOG_WARNING     警告
         * LOG_NOTICE      需要注意的特殊情况
         * LOG_INFO        一般信息
         * LOG_DEBUG       调试信息
         * 
         * syslog创建的日志信息包含消息头和消息体.消息头根据设施值及日期和时间创建.
         * 消息体根据syslog的message参数创建,转换控制符%m可用于插入与错误变量errno
         * 当前值对应的出错消息字符串.
         */
        syslog(LOG_ERR | LOG_USER, "oops - %m\n");
    }
    return 0;
}
