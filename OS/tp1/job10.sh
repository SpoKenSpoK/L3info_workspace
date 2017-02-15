#!/bin/bash

for ((I=0; I<10; I++)); do
./job.sh 90000 &
done
wait
exit 0
