#!/bin/bash
g++ -Wall -g -o sum main.cpp -fopenmp; 
a=0
d=0
e=1.2
f=1.2
N=5
for i in $(seq 1 $N);
do
	echo $d
	b=$(./sum)
	a=`echo $d+$b | bc`
	d=$a
done                                                                                                                                                   
#echo "scale=10; $a/$N" | bc

