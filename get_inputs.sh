#!/usr/bin/env sh
set -eu

if [ -f .env ]; then
    . ./.env
fi

if [ -z "$AOC_SESSION" ]; then
    echo "Set AOC_SESSION env var in shell or in .env file"
    exit 1
fi

mkdir -p input

get_input() {
    day_padded=$(printf "%02d" "$2")
    input_filename="input/$1-$day_padded.txt"
    curl -A "User-Agent: Mozilla/5.0" -H "Cookie: session=$AOC_SESSION" -o "$input_filename" "https://adventofcode.com/$1/day/$2/input"

    # Check if day is not available yet
    if head -n 1 "$input_filename" | grep -q "^Please don't"; then
        echo "$1 day $2 is not open"
        rm "$input_filename"
        return 1
    fi
}

for i in $(seq 1 25); do
    get_input 2025 "$i"
done
