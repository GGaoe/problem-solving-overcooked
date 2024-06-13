#!/bin/bash
if [ ! -z "$1" ]; then
    NUM=$1
else
    echo "Please provide the value of NUM."
    exit 1
fi
cd source
g++ main.cpp framework.cpp move.cpp -I ../include/
cd ..
#while true
#do
    #make fast LVL=5 NUM=$NUM
#done
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make fast LVL=5 NUM=$NUM
make run LVL=5 NUM=$NUM
