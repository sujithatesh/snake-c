#!/bin/bash

# Detect the operating system
OS=$(uname)

if [[ "$OS" == "Linux" ]]; then
    echo "Building for Linux..."
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/sujith/gitshit/raylib-5.0_linux_amd64/lib
    gcc snake.c -Wall -g -o snake \
    -L /home/sujith/gitshit/raylib-5.0_linux_amd64/lib \
    -I /home/sujith/gitshit/raylib-5.0_linux_amd64/include \
    -lraylib -lm \
    && ./snake

elif [[ "$OS" == "Darwin" ]]; then
    echo "Building for macOS..."
    gcc -v -Wall -Wextra -g snake.c -o snake \
    -L /Users/sujith.varkala/C/raylib/raylib-5.5_macos/lib \
    -I /Users/sujith.varkala/C/raylib/raylib-5.5_macos/include \
    -lraylib -lm \
    && export DYLD_LIBRARY_PATH=/Users/sujith.varkala/C/raylib/raylib-5.5_macos/lib:$DYLD_LIBRARY_PATH \
    && ./snake

else
    echo "Unsupported OS: $OS"
    exit 1
fi
