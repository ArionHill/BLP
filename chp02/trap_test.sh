#!/bin/sh#!/bin/sh

trap 'rm -f /tmp/my_tmp_file_$$' INT
echo creating file /tmp/my_tmp_file_$$
date > /tmp/my_tmp_file_$$

echo 'press interrupt (CTRL+C) to interrupt ...'
while [ -f /tmp/my_tmp_file_$$ ]; do
    echo File exists
    sleep 1
done

trap INT
echo creating file /tmp/my_tmp_file_$$
date > /tmp/my_tmp_file_$$
while [ -f /tmp/my_tmp_file_$$ ]; do
    echo File exists
    sleep 1
done
