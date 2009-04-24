sudo mkdir -p /usr/local/share/clam/layouts/
for ((a=15; a<23; a++))
do
	dummylayout=`pwd`/layouts/$a''_dummy.layout
	dummytriangulation=`pwd`/layouts/$a''_dummy.triangulation
	cp layouts/sixteen.triangulation  $dummytriangulation
	cat /dev/null > $dummylayout
	for ((speaker=0; speaker<a; speaker++))
	do
		echo 1 2 3 pica pared $a $speaker >> $dummylayout
	done
	sudo rm /usr/local/share/clam/layouts/$a.layout
	sudo rm /usr/local/share/clam/layouts/$a.triangulation
	sudo ln -s  $dummylayout /usr/local/share/clam/layouts/$a.layout
	sudo ln -s  $dummytriangulation /usr/local/share/clam/layouts/$a.triangulation
	./generateHoaDecoderNetwork.py /usr/local/share/clam/layouts/$a.layout 1 > ladspa/bformat$a.clamnetwork
	./generateVbapDecoderNetwork.py /usr/local/share/clam/layouts/$a.layout /usr/local/share/clam/layouts/$a.triangulation > ladspa/vbap$a.clamnetwork
done


