#!/bin/sh
BRANCH=$(git branch --show-current)
COMMIT=$(cat $1)
if grep -q -P "^(fix|feat|refactor|revert|perf):.*" "$1"; then
    printf "[$BRANCH]$COMMIT" > $1
    exit 0
else
    echo >&2 "Commit syntax is fix|feat|refactor|revert|perf: your commit text"
    exit 1
fi