#!/bin/bash

for i in $(seq 1 3)
    do
        echo "test$i..."
        valgrind --leak-check=full -q ./sokoban < przyklady/przyklad$i.in | diff - przyklady/przyklad$i.out
done
