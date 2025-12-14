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
check input/day03ex.txt 3 b 3121910778619
check input/day03.txt 3 b 172516781546707
check input/day04ex.txt 4 a 13
check input/day04.txt 4 a 1356
check input/day04ex.txt 4 b 43
check input/day04.txt 4 b 8713
check input/day05ex.txt 5 a 3
check input/day05.txt 5 a 756
check input/day05ex.txt 5 b 14
check input/day05.txt 5 b 355555479253787
check input/day06ex.txt 6 a 4277556
check input/day06.txt 6 a 6725216329103
check input/day06ex.txt 6 b 3263827
check input/day06.txt 6 b 10600728112865
