#!/bin/bash

# Run C Fast

t_run="run"

if [[ -z "$1" ]]; then
    echo "Missing arg: running file path"
    exit
fi

if [[ -n "$2" ]]; then
    t_run="$2"
fi

gcc "$1" -o ${t_run} && "./${t_run}"

echo