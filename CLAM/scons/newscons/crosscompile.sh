export SANDBOX=~/.wine/drive_c/GTK

die()
{
	echo $*
	exit -1
}


[ -d $SANDBOX ] || die "Please, define the SANDBOX variable in the script."

#export PKG_CONFIG_PATH=$SANDBOX/lib/pkgconfig/
#export PKG_CONFIG=$SANDBOX/bin/pkg-config.exe

echo PKG_CONFIG $PKG_CONFIG
echo PKG_CONFIG_PATH $PKG_CONFIG_PATH

#export PKG_CONFIG_SYSROOT_DIR=$SANDBOX
export PKG_CONFIG_PATH= 
export PKG_CONFIG_LIBDIR=$SANDBOX/lib/pkgconfig

scons --target=mingw verbose=1 prefix=mingw-usr $* # --config=force 



