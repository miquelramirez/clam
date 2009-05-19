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
sudo rm /usr/local/share/clam/layouts/22.layout
sudo rm /usr/local/share/clam/layouts/22.triangulation
sudo ln -s  `pwd`/layouts/14_audiolab.layout /usr/local/share/clam/layouts/14.layout
sudo ln -s  `pwd`/layouts/14_audiolab.triangulation /usr/local/share/clam/layouts/14.triangulation
sudo ln -s  `pwd`/layouts/15_audiolab.layout /usr/local/share/clam/layouts/15.layout
sudo ln -s  `pwd`/layouts/15_audiolab.triangulation /usr/local/share/clam/layouts/15.triangulation
sudo ln -s  `pwd`/layouts/22_twyford_guessed.layout /usr/local/share/clam/layouts/22.layout
sudo ln -s  `pwd`/layouts/22_twyford_guessed_stereographic.triangulation /usr/local/share/clam/layouts/22.triangulation

