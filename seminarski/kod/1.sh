#!/bin/bash

flex 1.l
gcc lex.yy.c
./a.out < 1.cpp > 1_copy.cpp
./a.out < test.cpp > test_copy.cpp
diff 1_copy.cpp test_copy.cpp
