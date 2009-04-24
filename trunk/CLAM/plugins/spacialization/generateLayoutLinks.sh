sudo mkdir -p /usr/local/share/clam/layouts/
for ((a=15; a<23; a++))
do
	dummylayout=`pwd`/layouts/$a''_dummy.layout
	dummytriangulation=`pwd`/layouts/$a''_dummy.triangulation
	sudo rm /usr/local/share/clam/layouts/$a.layout
	sudo rm /usr/local/share/clam/layouts/$a.triangulation
	sudo ln -s  $dummylayout /usr/local/share/clam/layouts/$a.layout
	sudo ln -s  $dummytriangulation /usr/local/share/clam/layouts/$a.triangulation
done
sudo rm /usr/local/share/clam/layouts/15.layout
sudo rm /usr/local/share/clam/layouts/15.triangulation
sudo ln -s  `pwd`/layouts/sixteen.layout /usr/local/share/clam/layouts/15.layout
sudo ln -s  `pwd`/layouts/sixteen.triangulation /usr/local/share/clam/layouts/15.triangulation

