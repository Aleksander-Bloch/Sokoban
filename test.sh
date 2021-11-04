#!/bin/bash

for i in $(seq 1 10)
    do
        echo "test$i..."
        valgrind --leak-check=full -q ./sokoban < testy/test$i.in | diff - testy/test$i.out
done
