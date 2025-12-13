#!/usr/bin/env sh
set -eu

cd "$(dirname "$(readlink -f -- "$0")")"

pwd
. ./dev_shell.sh
bld

# Example shellcheck disable
## shellcheck disable=SC2086 # allow word splitting

check() {
    echo "$1" "$2" "$3"
    ./build/app/aoc-app "$1" "$2" "$3" --expect "$4"
}

check input/day01.txt 1 a 969
check input/day01.txt 1 b 5887
check input/day02ex.txt 2 a 1227775554
check input/day02.txt 2 a 40214376723
check input/day02ex.txt 2 b 4174379265
check input/day02.txt 2 b 50793864718
check input/day03ex.txt 3 a 357
check input/day03.txt 3 a 17332
