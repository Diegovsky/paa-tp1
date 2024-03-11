#!/usr/bin/bash
num="${1:-10}"
expected=$(python -c "print(sum(range($num+1)))")
result=$((echo $num ; seq $num) | ./tp1)
echo $expected == $result
