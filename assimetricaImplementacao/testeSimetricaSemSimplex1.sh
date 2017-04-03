#!/bin/bash

for ((i=0; i <= 30; i++))
do 
	./transgenetico < ../Intances/Adaptadas/Simetrica/berlin52.tsp > ../Intances/Adaptadas/Simetrica/berlin52/berlin52_"$i".out
	echo "Done $i"
done