#!/bin/bash

function prendre_peripherique()
{
	local nom="$1"
	while ! mkdir "$nom" 2>/dev/null;do
		sleep 0.01s
	done
	rm -r $1
}


prendre_peripherique toto
for((I=0; I< $1; I++))
do J=I
done
date +%s
prendre_peripherique toto
exit 0
