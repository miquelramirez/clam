prefix = @prefix@
libdir = @prefix@/lib
includedir = @prefix@/include

Name: @PACKAGE_NAME@
Description: @PACKAGE_DESCRIPTION@
Version: @PACKAGE_VERSION@
Requires: @PACKAGE_DEPS@
Conflicts:
Libs: -L${libdir} -l@PACKAGE_NAME@
Cflags: -I${includedir} @PACKAGE_DEFINES@ -include ${includedir}/@PACKAGE_PRE_INCLUDE@