#!/bin/bash
START=1
if [ $# -eq 0 ]
  then
    echo "No arguments supplied, using 120 frames..."
    END=120
  else
  	END=$1
fi

echo -n "Creating .dat files..."
python create.py $END
echo " .dat files created."
for i in `seq $START $END`;
do
	echo -en "\e[1K\rGenerating mandelbox image $i/$END...";
    ./mandelbox ./params$i.dat;
    mogrify -format jpg image$i.bmp;
    rm image$i.bmp
done
echo " Finished rendering images."
echo -n "Removing .dat files..."
echo "rm params*.dat"
echo " .dat files removed."
echo "Generating video..."
ffmpeg -start_number 1 -i image%d.jpg -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4
echo -n "Done!"
vlc out.mp4
