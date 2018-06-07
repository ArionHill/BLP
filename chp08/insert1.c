#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

int main(int argc, char const *argv[])
{
    MYSQL *connection;
    int res;

    connection = mysql_init(NULL);
    if (mysql_real_connect(connection, "localhost", "rick", "rick", "foo", 0, NULL, 0)) {
        printf("Connection success\n");

        res = mysql_query(connection, "INSERT INTO children(fname, age)VALUES('Ann', 3)");

        if (!res) {
            printf("Inserted %lu rows\n",
                            (unsigned long)mysql_affected_rows(connection));
        } else {
            fprintf(stderr, "Insert error %d: %s\n", mysql_errno(connection),
                                                    mysql_error(connection));
        }

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
