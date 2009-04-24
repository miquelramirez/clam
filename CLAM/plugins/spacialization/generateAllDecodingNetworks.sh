function random()
{
	min=$1
	max=$2
	echo $(( min+(max-min)*RANDOM/32767 ))
}


sudo mkdir -p /usr/local/share/clam/layouts/
for ((a=15; a<=22; a++))
do
	dummylayout=`pwd`/layouts/$a''_dummy.layout
	dummytriangulation=`pwd`/layouts/$a''_dummy.triangulation
	cp layouts/sixteen.triangulation  $dummytriangulation
	cat /dev/null > $dummylayout
	for ((speaker=0; speaker<a; speaker++))
	do
		echo `random -90 90` `random 0 360` dummy $a $speaker >> $dummylayout
	done
	sudo rm /usr/local/share/clam/layouts/$a.layout
	sudo rm /usr/local/share/clam/layouts/$a.triangulation
	sudo ln -s  $dummylayout /usr/local/share/clam/layouts/$a.layout
	sudo ln -s  $dummytriangulation /usr/local/share/clam/layouts/$a.triangulation
	./generateHoaDecoderNetwork.py /usr/local/share/clam/layouts/$a.layout 1 > ladspa/bformat$a.clamnetwork
	./generateVbapDecoderNetwork.py /usr/local/share/clam/layouts/$a.layout /usr/local/share/clam/layouts/$a.triangulation > ladspa/vbap$a.clamnetwork
done


