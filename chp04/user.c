#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    uid_t uid;
    gid_t gid;

    struct passwd *pw;
    /**
     * #include <sys/types.h>
     * #include <unistd.h>
     * uid_t getuid(void)
     * char *getlogin(void)
     * getuid返回程序关联的UID, 通常是启动程序的用户的UID,UID有自己的类型uid_t,
     * 定义在sys/types.h中.通常是一个小整数
     * getlogin返回与当前用户关联的登录名.
     */
    uid = getuid();
    gid = getgid();

    printf("User is %s\n", getlogin());

    printf("User IDs: uid=%d, gid=%d\n", uid, gid);

    /**
     * #include <sys/types.h>
     * #include <pwd.h>
     * struct passwd *getpwuid(uid_t uid);
     * 数据库结构passwd定义在头文件pwd.h中.包含成员如下:
     * char *pw_name: 用户登录名
     * uid_t pw_uid: UID号
     * gid_t pw_gid: GID号
     * char *pw_dir: 用户家目录
     * char *pw_gecos: 用户全名
     * char *pw_shell: 用户默认shell
     * 在某些系统上用户全名为pw_gecos,而其它系统上可能是pw_comment.
     * 两个函数成功时返回与用户对用的passwd结构.出错时返回空指针并设置
     * errno
     */
    pw = getpwuid(uid);
    printf("UID passwd entry:\n name=%s, uid=%d, gid=%d, home=%s, fullname=%s, shell=%s\n", 
        pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_gecos, pw->pw_shell);
    /**
     * #include <sys/types.h>
     * #include <pwd.h>
     * struct passwd *getpwnam(uid_t uid);
     */
    pw = getpwnam("root");
    printf("root passwd entry:\n");
    printf("name=%s, uid=%d, gid=%d, home=%s, fullname = %s, shell=%s\n",
        pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_gecos, pw->pw_shell);
    exit(0);
}