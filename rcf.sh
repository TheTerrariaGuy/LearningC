#!/bin/bash

# Run C Fast

base_dir="/home/samson/Coding-WSL/CLearning/"

if [[ -z "$1" ]]; then
    echo "Missing arg: running file path"
    exit
fi

scripts=""

for arg in "$@"
do
    if [[ -d "$arg" ]]; then
        for file in "$arg"/*.c;
        do
            [[ -n "$scripts" ]] && scripts+=" "
            scripts+="$base_dir$file"
            
        done
    elif [[ "$arg" == *.c ]]; then 
        [[ -n "$scripts" ]] && scripts+=" "
        scripts+="$base_dir$arg"
    fi
done

gcc "$scripts" -o run && "./run"

echo