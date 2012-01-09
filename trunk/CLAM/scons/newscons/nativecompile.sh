echo PKG_CONFIG $PKG_CONFIG
echo PKG_CONFIG_PATH $PKG_CONFIG_PATH

scons --config=force verbose=1 prefix=usr/ $*



