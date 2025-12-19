#!/usr/bin/env sh
set -eu

cd "$(dirname "$(readlink -f -- "$0")")"

pwd
. ./dev_shell.sh
bld

check() {
    echo "$1" "$2" "$3"
    ./build/app/aoc-app "$1" "$2" "$3" --expect "$4"
}

check input/2025-01.txt 01 a 969
check input/2025-01.txt 01 b 5887
check input/2025-02.txt 02 a 40214376723
check input/2025-02.txt 02 b 50793864718
check input/2025-03.txt 03 a 17332
check input/2025-03.txt 03 b 172516781546707
check input/2025-04.txt 04 a 1356
check input/2025-04.txt 04 b 8713
check input/2025-05.txt 05 a 756
check input/2025-05.txt 05 b 355555479253787
check input/2025-06.txt 06 a 6725216329103
check input/2025-06.txt 06 b 10600728112865
check input/2025-07.txt 07 a 1535
check input/2025-07.txt 07 b 4404709551015
