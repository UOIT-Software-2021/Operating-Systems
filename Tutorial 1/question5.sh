#!/bin/bash
if [$1 -eq 'loop']
    then
        echo 'loop'
fi
for i in {1..5}
do
    echo $1
    echo i > count.log
    ps -a > processes.log
done
