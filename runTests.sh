#!/bin/sh


check()
{
        output=$(mktemp)
        ./src/calc -c -q -s -f $1 > $output
        if diff $output $2; then
	        echo tests passed
        else
	        echo tests failed
        fi
        rm $output
}

for src in $(ls tests/*.calc); do
        output=$(echo $src | sed -e 's/.calc/.output/')
        check $src $output
done

