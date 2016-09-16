#!/bin/bash

gcc -g -c poly.c -o poly.o 
gcc -g -Wall -Werror  test.c -o test poly.o
./test > generate.c
