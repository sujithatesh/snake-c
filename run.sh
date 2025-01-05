#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/sujith/gitshit/real-raylib/src
gcc snake.c -Wall -g -o snake \
-L /home/sujith/gitshit/real-raylib/src \
-I /home/sujith/gitshit/real-raylib/src \
-lraylib -lm  \
&& ./snake
