#!/bin/bash
#	A+B=m	A+3B=c
m=`wc -m $1 | cut -d' ' -f 1`
c=`wc -c $1 | cut -d' ' -f 1`
B=$(( ( $c - $m ) / 2 ))
A=$(( $m - $B ))
echo -e "\t\tWord Count"
echo -e "\tNon-Chinese:\t$A"
echo -e "\tChinese:\t$B"
echo -e "\tTotal:\t\t$(( $A + $B ))"

