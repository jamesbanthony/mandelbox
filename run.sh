#!/bin/bash
START=$1
END=$2

python create.py $1 $2
for i in `seq $START $END`;
do
	echo working on $i;
    ./mandelbox ./params$i.dat
done
rm params*.dat
ffmpeg -f image2 -r 30 -i image%d.bmp ./out.mov
