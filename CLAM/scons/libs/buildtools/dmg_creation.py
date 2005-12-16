import os

def create_dmg( target, source, env) :
	os.system( """
echo " Creating temporary directories" ;
mkdir -p DMG/include/CLAM ;
mkdir -p DMG/lib/pkgconfig ;
cp -f `find . | grep "\.h"` DMG/include/CLAM/ ;
cp -R `find . | grep libclam` DMG/lib/ ;
cp `find . | grep "\.pc"` DMG/lib/pkgconfig/ ;

echo " Compressing files" ;
cd DMG/include ;
tar cfjv ../headers.tbz2 CLAM ;
cd .. ;
rm -rf include ;

cd lib ;
tar cfjv ../libraries.tbz2 * ;
cd .. ;
rm -rf lib ;

echo " Copying resource files" ;
cd .. ;
cp resources_mac/README.txt DMG ;
cp resources_mac/Install.command DMG ;

echo " Creating DMG package" ;
sudo hdiutil create -srcfolder DMG -volname CLAM-libs -uid 0 CLAM-libs-%s.dmg ;
rm -rf DMG ;

echo " Success!" ;
"""%target[0] )

def create_dmg_message( target, source, env):
	return "Creating DMG package"

