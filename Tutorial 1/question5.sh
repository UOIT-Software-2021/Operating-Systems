#!/bin/bash
for i in {1..5}
do
    echo $1
    echo i > count.log
    ps 
done
