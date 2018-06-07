#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

int main(int argc, char const *argv[])
{
    MYSQL *connection;

    connection = mysql_init(NULL);
    // connection = mysql_real_connect(connection, "localhost", "rick", "null", "rick", 0, NULL, 0);
    if (mysql_real_connect(connection, "localhost", "rick", "null", "rick", 0, NULL, 0)) {
        printf("Connection success\n");
        mysql_close(connection);
    } else {
        printf("Connection failed %d\n", mysql_errno(connection));
        if (mysql_errno(connection)) {
            fprintf(stderr, "Connection error %d: %s\n",
                    mysql_errno(connection), mysql_error(connection));
        }
    }

    
    return 0;
}
