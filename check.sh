#!/usr/bin/env sh
set -eu

cd "$(dirname "$(readlink -f -- "$0")")"

# Example shellcheck disable
## shellcheck disable=SC2086 # allow word splitting

check() {
    echo "$1" "$2" "$3"
    ./build/app/aoc-app "$1" "$2" "$3" --expect "$4"
}

check input/day01.txt 1 a 969
check input/day01.txt 1 b 5887
