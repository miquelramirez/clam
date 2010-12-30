#!/usr/bin/python

"""
This scripts setups clam dependencies to generated Windows
mingw compatible binaries from a Linux box (crosscompilation).

Non complete list of requirements (Ubuntu):
sudo apt-get install gcc-mingw32 mingw32-binutils mingw32-runtime \
	nsis wine automake autoconf-archive libtool pkg-config \
	wget sed autogen mm-common bjam

Please, update the list of packages if you find any missing one.
Note that 'ming32' package is legacy 3.2.1 gcc, do not install it.

You can provide the name of a package to download and build,
or you can just build all of them.


Add packages by adding calls to 'package'.

If your package is named 'foo' and you add patch files matching
mingw-foo*.patch to this directory, they will be applied in
alphabetical order to the downloaded and extracted source.


Package TODO:
Legend: - todo, * working on it, + done, X failed, child means dependency
- libid3 lacks a pkg-config file
- Test whether LT_INIT([win32-dll]) in configure.ac helps to generate the dlls for libid3 and others
- libsndfile lacks dependencies: sqlite flac ogg vorbis
- USe DX9 or DX10 instead of DX8
- properly support flac (does not link in sndfile)
- ladspa-sdk::applyplugin binary had to be disabled
+ libxml++
	+ libxml2
	+ glibmm
		+ libsigc++
		+ glib
			+ gettext
- lv2core
	- lv2-ui
	- lv2 other extensions
+ boost
X libpython
* qt: supported by mingw-cross-env
	+ libpng
		+ zlib
	+ libmng
		+ jpeg
		+ lcms
- Asio, Vst
- Seems to be a race condition in the clam test program for pthreads that makes it segfault sometimes


Script TODO:
+ Target by command line
- Options for prefix, target...
- Config file
- Declarative + dependencies
"""


import subprocess
import sys
import os
import glob


sandbox = os.path.expanduser("~/mingw32")
target = "i586-mingw32msvc"
sfmirror = "http://puzzle.dl.sourceforge.net"
apachemirror = "http://www.apache.org/dist"
apachemirror = "http://apache.rediris.es/"
prefix = os.path.join(sandbox,"local")
skipDeploy = False

def loadDictFile(dictfile) :
	""" Returns a dict with the variables defined in a file """
	class temp : exec(open(dictfile))
	loaded = dict(temp.__dict__)
	del loaded['__doc__']
	del loaded['__module__']
	return loaded

def die(message) :
	""" Exits the program by prompting a message using the do-this-or-die idiom. """
	print >> sys.stderr, "\033[31m%s\033[0m"%message
	sys.exit(-1)

def warning(message) :
	""" Outputs a warning message. """
	print >> sys.stderr, "\033[33m%s\033[0m"%message

def stage(message) :
	""" Outputs an stage message. """
	print >> sys.stderr, "\033[35m=== %s === \033[0m"%message

class tee :
	""" Output file decorator that duplicates the output to two files. """
	def __init__(self, file1, file2) :
		self.f1=file1
		self.f2=file2
	def flush(self):
		self.f1.flush()
		self.f2.flush()
	def write(self, content) :
		self.f1.write(content)
		self.f2.write(content)

class quotedFile :
	""" Output file decorator that, surrounds the content of each write
	in between 'incode' and 'outcode'. """
	def __init__(self, aFile, incode, outcode) :
		self.incode = incode
		self.outcode = outcode
		self.f = aFile
	def write(self,content) :
		self.f.write(self.incode)
		self.f.write(content)
		self.f.write(self.outcode)
	def flush(self):
		self.f.flush()

def run(command, message=None, log=sys.stdout, err=None) :
	if not message : message = "Running: " + command
	print "\033[32m== %s\033[0m"%(message)
	if err is None :
		err = quotedFile(log, "\033[31m", "\033[0m")
	process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	import select
	outpoll = select.poll()
	outpoll.register(process.stdout.fileno())
	outpoll.register(process.stderr.fileno())
	while process.returncode is None :
		process.poll()
		for fd, flags in outpoll.poll() :
			if fd==process.stderr.fileno() :
				err.write(process.stderr.readline())
			if fd==process.stdout.fileno() :
				log.write(process.stdout.readline())
	if process.returncode : die("Failed, exit code %i"%process.returncode)

def output(command, message=None) :
	if message :
		print "\033[32m== %s\033[0m"%(message)
	return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).communicate()[0] 

def applyPatch(directory, patch, level=0) :
	fullpatchpath = scriptRelative(patch)
	run("patch -d '%s' -p%i < '%s' "%(directory, level, fullpatchpath),
		"Applying patch %s"%os.path.basename(patch)
		)

def download(uri, filename=None) :
	if filename:
		outputOption = " -O '%s'"% os.path.join(sandbox,"downloads",filename)
	else :
		outputOption = " -P '%s'"% os.path.join(sandbox,"downloads")
	run("wget --progress=dot -c %s '%s'" % (outputOption, uri),
		"Downloading %s"%uri
	)

def extractSource(tarball) :
	def extractCommand(tarball) :
		for extension, command in [
			('.tar.gz',  "pv -p %(sandbox)s/downloads/%(tarball)s | tar xz -C %(sandbox)s/src"),
			('.tgz',     "pv -p %(sandbox)s/downloads/%(tarball)s | tar xz -C %(sandbox)s/src"),
			('.tar.bz2', "pv -p %(sandbox)s/downloads/%(tarball)s | tar xj -C %(sandbox)s/src"),
			('.zip',     "unzip -xo %(sandbox)s/downloads/%(tarball)s -d %(sandbox)s/src/%(basename)s"),
			] :
			if tarball.endswith(extension) : return extension, command
		die("Unsuported compressed file extension: %s" % tarball)
	extension, command = extractCommand(tarball)
	basename = tarball[:-len(extension)]
	print basename
	run( 
		command % dict(
			sandbox=sandbox,
			tarball=tarball,
			basename=basename,
		),
		"Uncompressing %s"%tarball)

def ensureDir(adir) :
	try: os.makedirs(adir)
	except OSError :  pass

def scriptRelative(path) :
	return os.path.join(os.path.dirname(os.path.abspath(__file__)), path)

def sfCheckVersion(project, package) :
	return (
		""" wget -q -O- 'http://sourceforge.net/projects/%s/files/%s/' | """
		""" grep legacy_release_notes | """
		""" grep download_url |  """
		""" sed -e 's,.*"\([0-9][^"]*\)":.*,\\1,' """
#		""" grep 'title="/' | """
#		""" sed -n 's,.*title="\(/[^:]*\).*released on \([0-9-]*\)",\\2 \\1,p' | """
#		""" sort | """
#		""" sed 's,^[^ ]* ,,' | """
		)%(project, package)

def gnomeCheckVersion(name, majorVersion) :
	return (
		""" wget -q -O- 'http://git.gnome.org/browse/%(name)s/refs/tags' | """
		""" grep '<a href=' | sed -n 's,.*<a[^>]*>\([0-9][^<]*\)<.*,\\1,p' | """
		""" grep %(majorVersion)s | """ # downloadUri limits us 
		""" sort | """
		""" tail -1 """
		% locals() )

def buildPackage(name, uri, checkVersion, downloadUri, tarballName, buildCommand,
		srcdir=None,
		deps="",
		pinnedVersion = None,
		checkout = None, # checkout command line
	) :
	stage("Module %s"%name)
	subst = dict(
		sandbox = sandbox,
		prefix = prefix,
		target = target,
		sfmirror = sfmirror,
		apachemirror = apachemirror,
		name = name,
		pinned = pinnedVersion if pinnedVersion else "None",
	)
	availableVersion = output(checkVersion, "Checking version for %s"%name).strip()
	availableVersion or die("No online version found for the package\n Command used:\n%s" % checkVersion)
	print "Found version: '%s'" % availableVersion
	if (pinnedVersion and pinnedVersion != availableVersion) :
		warning("Package: Pinning to version %s, although version %s is available" % (pinnedVersion, availableVersion))
	version = pinnedVersion if pinnedVersion else availableVersion
	subst.update (
		version = version,
		majorversion = ".".join(version.split(".")[:1]),
		minorversion = ".".join(version.split(".")[:2]),
		minorversionnumber = ".".join(version.split(".")[1:2]),
	)

	subst.update(
		tarball = tarballName % subst,
		srcdir = ("%(sandbox)s/src/" + (srcdir or "%(name)s-%(version)s/")) % subst,
	)
	print "srcdir:", subst['srcdir']

	if not skipDeploy :
		download(downloadUri % subst)
		extractSource(subst['tarball'])
		patches = glob.glob(scriptRelative("mingw-"+name+"*.patch"))
		patches.sort()
		for patch in patches :
			applyPatch(subst['srcdir'], patch, level=1)

	log = tee(open("log-%s-build"%name,'w'), sys.stdout)
	run(buildCommand % subst, log=log)
	run("touch finnished-%(name)s-%(version)s"%subst)

packageDatabase = {}
packageOrder = []

def package(name, **keywordArgs) :
	packageDatabase[name] = keywordArgs
	packageOrder.append(name)


def build(names) :
	for name in names.split() :
		buildPackage(name, **packageDatabase[name])

def buildAll() :
	for name in packageOrder : build(name)




##########################################################################################################3


ensureDir(os.path.join(sandbox, "src"))
ensureDir(os.path.join(sandbox, "downloads"))
ensureDir(os.path.join(prefix, "include"))
ensureDir(os.path.join(prefix, "lib"))
ensureDir(os.path.join(prefix, "lib", "pkgconfig"))
ensureDir(os.path.join(prefix, "bin"))

os.environ.update(
	PKG_CONFIG_LIBDIR = "/usr/%s/lib/pkgconfig"%target, # default debian installation
	PKG_CONFIG_PATH = os.path.join(prefix, 'lib', 'pkgconfig'), # our prefix installation
#	PKG_CONFIG_ALLOW_SYSTEM_CFLAGS = "1", # do not strip system include path
#	PKG_CONFIG_ALLOW_SYSTEM_LIBS = "1", # do not strip system lib path
	)


package( "pthread",
	uri = "http://sourceware.org/pthreads-win32/",
	checkVersion = ""
		""" wget -q -O- 'ftp://sourceware.org/pub/pthreads-win32/Release_notes' | """
		""" sed -n 's,^RELEASE \([0-9][^[:space:]]*\).*,\\1,p' | """
		""" tr '.' '-' | """
		""" head -1 """,
	tarballName = "pthreads-w32-%(version)s-release.tar.gz",
	downloadUri = "ftp://sourceware.org/pub/pthreads-win32/%(tarball)s",
	srcdir = "pthreads-w32-%(version)s-release",
	buildCommand =
		""" cd %(srcdir)s && """
		""" make CROSS=i586-mingw32msvc- GC-inlined && """
		""" mkdir -p %(prefix)s/include && """
		""" mkdir -p %(prefix)s/lib && """
		""" mkdir -p %(prefix)s/bin && """
		""" cp libpthreadGC2.a %(prefix)s/lib && """
		""" cp *.h %(prefix)s/include && """
		""" cp pthreadGC2.dll %(prefix)s/bin && """
		""" ln -sf libpthreadGC2.a %(prefix)s/lib/libpthread.a """
	)


package( "fftw",
	uri = "http://www.fftw.org",
	deps = "pthread",
	checkVersion =
		""" wget -q -O- 'http://www.fftw.org/download.html' | """
		""" sed -n 's,.*fftw-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" grep -v alpha | """
		""" grep -v beta | """
		""" head -1 """,
	tarballName = "fftw-%(version)s.tar.gz",
	downloadUri = "http://www.fftw.org/%(tarball)s",
	buildCommand =
		"cd %(srcdir)s && "
		"autoconf && "
		"./configure --host='%(target)s' --prefix='%(prefix)s' && "
		"make install "
	)


# TODO: the pkg-config patch does not applies twice as it creates a file
package( 'libmad',
	uri = "http://www.underbit.com/products/mad/",
	checkVersion =
		""" wget -q -O- 'http://sourceforge.net/projects/mad/files/libmad/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	tarballName = "libmad-%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/sourceforge/mad/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i '/-fforce-mem/d' configure.ac && """
		""" touch NEWS AUTHORS ChangeLog && """
		""" autoreconf --force --install && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """
		""" make install """
	)

package( "id3lib",
	uri = "http://id3lib.sourceforge.net/",
	deps = "zlib iconv",
	checkVersion =
		""" wget -q -O- 'http://sourceforge.net/projects/id3lib/files/id3lib/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	downloadUri = "%(sfmirror)s/sourceforge/id3lib/%(tarball)s",
	tarballName = "id3lib-%(version)s.tar.gz",
	buildCommand =
		""" cd %(sandbox)s/src/id3lib-%(version)s && """
		""" aclocal -I m4 && """
		""" libtoolize && """
		""" autoconf && """
		""" automake && """
		""" CPPFLAGS='-I%(prefix)s/include' """ # Needed for: zlib, iconv... TODO: investigate dlfcn usage
		""" LDFLAGS='-L%(prefix)s/lib' """
		""" DLLTOOL=%(target)s-dlltool """
		""" OBJDUMP=%(target)s-objdump """
		""" AS=%(target)s-as """
			""" ./configure --host='%(target)s' --prefix='%(prefix)s' --enable-shared && """
		""" make && """
		""" make install """
	)


package( "libogg",
	uri = 'http://www.xiph.org/ogg/',
	checkVersion =
		""" wget -q -O- 'http://www.xiph.org/downloads/' | """
		""" sed -n 's,.*libogg-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "libogg-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/ogg/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i 's,cross_compiling=no,cross_compiling=yes,' 'configure' && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """
		""" make && """
		""" make install """
	)


package( "libvorbis",
	uri = "http://www.vorbis.com/",
	deps = "libogg",
	checkVersion =
		""" wget -q -O- 'http://www.xiph.org/downloads/' | """
		""" sed -n 's,.*libvorbis-\([0-9][^>]*\)\.tar.*,\\1,p' |  """
		""" head -1 """,
	downloadUri = "http://downloads.xiph.org/releases/vorbis/libvorbis-%(version)s.tar.gz",
	tarballName = "libvorbis-%(version)s.tar.gz",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """ 
		""" make && """
		""" make install """
	)

package( "gettext",
	uri = "http://www.gnu.org/software/gettext/",
	checkVersion = 
		""" wget -q -O- 'http://www.gnu.org/software/gettext/' | """
		""" grep 'gettext-' | """
		""" sed -n 's,.*gettext-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "ftp://ftp.gnu.org/pub/gnu/gettext/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" cd gettext-runtime && """
		""" ./configure """
			""" --host='%(target)s' """
			""" --prefix='%(prefix)s' """
			""" --enable-threads=win32 """
			""" --disable-java """
			""" --without-git """
			""" --without-libexpat-prefix """
			""" --without-libxml2-prefix """
			""" CONFIG_SHELL=/bin/bash """
			""" && """
		""" make install """
	)

package( "libiconv",
	uri = "http://www.gnu.org/software/libiconv/",
	deps = 'gettext',
	checkVersion =
		""" wget -O- -q 'http://ftp.gnu.org/pub/gnu/libiconv/?C=M;O=A' | """
		""" sed -n 's,.*libiconv-\([0-9][^<]*\)\.tar.*,\\1,p' | """
		""" tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://ftp.gnu.org/pub/gnu/%(name)s/%(tarball)s",
	buildCommand = 
		""" cd %(srcdir)s && """
		""" sed -i 's, sed , sed ,g' 'windows/windres-options'  && """ # TODO useless command?
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' """
#			""" --disable-nls """
			""" && """
		""" make install """
		# cross-mingw-env uses this:
#		""" make -C 'libcharset' install && """
#		""" make -C 'lib'        install && """
#		""" $(INSTALL) -d '%(prefix)s/include' && """
#		""" $(INSTALL) -m644 'include/iconv.h.inst' '%(prefix)s/include/iconv.h' && """
	)

	
# TODO: flac dll has missing symbols when linking it against sndfile
package( "flac",
	uri = "http://flac.sourceforge.net/",
	deps = "libiconv libogg pthread",
	checkVersion =
		""" wget -q -O- 'http://flac.cvs.sourceforge.net/viewvc/flac/flac/' | """
		""" grep '<option>FLAC_RELEASE_' | """
		""" sed -n 's,.*FLAC_RELEASE_\([0-9][0-9_]*\)__.*,\\1,p' | """
		""" sed 's,_,.,g' | """
		""" head -1 """,
	tarballName = "flac-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/flac/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./autogen.sh --host='%(target)s' --prefix='%(prefix)s' """
			""" PKG_CONFIG_PATH=$PKG_CONFIG_PATH """
			""" --disable-xmms-plugin """
			""" --enable-ogg """
			""" --disable-oggtest """
			""" && """
		""" make && """
		""" make install """
	)

package( "libsndfile",
	uri = "http://www.mega-nerd.com/libsndfile/",
	deps = "ogg vorbis flac",
	pinnedVersion = "1.0.21",
	checkVersion =
		""" wget -q -O- 'http://www.mega-nerd.com/libsndfile/' | """
		""" grep '<META NAME="Version"' | """
		""" sed -n 's,.*CONTENT="libsndfile-\([0-9][^"]*\)">.*,\\1,p' | """
		""" head -1 """,
	tarballName = "libsndfile-%(version)s.tar.gz",
	downloadUri = "http://www.mega-nerd.com/libsndfile/files/libsndfile-%(version)s.tar.gz",
	buildCommand = 
		""" cd %(srcdir)s && """
#		""" autoconf && """
		""" ./configure mingw32 --host=%(target)s --prefix=%(prefix)s """
			""" --disable-external-libs """
#			""" --disable-flac """
			""" --disable-sqlite """
			""" --disable-octave """
			""" --disable-alsa """
			""" && """
		""" make install """
	)

package( "speex",
	uri = "http://www.speex.org/",
	deps = "libogg",
	checkVersion = 
		""" wget -q -O- 'http://git.xiph.org/?p=speex.git;a=tags' | """
		""" grep '<a class="list name"' | """
		""" sed -n 's,.*<a[^>]*>Speex-\([0-9][^<]*\)<.*,\\1,p' | """
		""" head -1 """,
	tarballName = "speex-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/speex/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		"""  ./configure  --host=%(target)s --prefix=%(prefix)s """
			""" --with-ogg-dir=%(prefix)s """
#			""" --enable-sse && """ # TODO: says 'not supported by arch'
			""" && """
		""" make && """
		""" make install """
	)

package( "liblo",
	uri = "http://liblo.sourceforge.net/",
	deps = "pthreads",
	checkVersion =
		""" wget -q -O- 'http://sourceforge.net/projects/liblo/files/liblo/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/sourceforge/liblo/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./autogen.sh  --host=%(target)s --prefix=%(prefix)s """
			""" LDFLAGS="-L%(prefix)s/lib/" """
			""" CPPFLAGS="-I%(prefix)s/include/" """
			""" && """
		""" make && """
		""" make install """
	)

# TODO: send patch modifications back to fedora
package( "zlib",
	uri = "http://zlib.net/",
	deps = "",
	checkVersion = 
		""" wget -q -O- 'http://zlib.net/' | """
		""" sed -n 's,.*zlib-\([0-9][^>]*\)\.tar.*,\\1,ip' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://zlib.net//%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" mkdir -p m4 && """
		""" autoreconf --install && """
		""" ./configure  --prefix='%(prefix)s' --host='%(target)s' && """
		""" make all && """
		""" make install && """
		""" echo Package %(name)s done."""
	)

package( "libpng",
	uri = "http://www.libpng.org",
	deps = "zlib",
	checkVersion = 
		""" wget -q -O- 'http://libpng.git.sourceforge.net/git/gitweb.cgi?p=libpng/libpng;a=tags' | """
		""" grep '<a class="list name"' | """
		""" sed -n 's,.*<a[^>]*>v\([0-9][^<]*\)<.*,\\1,p' | """
		""" grep -v alpha | """
		""" grep -v beta | """
		""" grep -v rc | """
		""" grep -v '^1\.2\.' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/project/%(name)s/%(name)s%(majorversion)s%(minorversionnumber)s/%(version)s/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
	#	""" ./autogen.sh && """
	#	""" aclocal && """
	#	""" autoconf && """
	#	""" libtoolize --force && """
		""" ./configure  --prefix='%(prefix)s' --host='%(target)s' """
			""" CFLAGS='-I%(prefix)s/include' """
			""" LDFLAGS='-L%(prefix)s/lib' """
			""" LIBS='-lz' """
			""" && """ 
		""" make install """
	)

package( "cppunit",
	uri = "http://liblo.sourceforge.net/",
	checkVersion =
		""" wget -q -O- 'http://sourceforge.net/projects/cppunit/files/cppunit/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/sourceforge/cppunit/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure --host=%(target)s --prefix=%(prefix)s  """
			""" --enable-doxygen=no """
			""" && """
		""" make && """
		""" make install """
#		""" cp src/cppunit/.libs/libcppunit-1-12-0.dll ../../dlls""" # TODO: Needed?
	)
	# TODO: From the wiki: To get some extra autoconf macros and libtool do: 
	# sudo apt-get install autoconf-archive libtool automake


package( "bzip2",
	uri = "http://www.bzip.org",
	deps = "",
	checkVersion = 
		""" wget -q -O- 'http://www.bzip.org/downloads.html' | """
		""" grep 'bzip2-' | """
		""" sed -n 's,.*bzip2-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://www.bzip.org/%(version)s/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i 's,sys\\\\stat\\.h,sys/stat.h,g' 'bzip2.c' && """
		""" sed -i 's,WINAPI,,g'                 'bzlib.h' && """
		""" make install """
			""" PREFIX='%(prefix)s' """
			""" CC='%(target)s-gcc' """
			""" AR='%(target)s-ar' """
			""" RANLIB='%(target)s-ranlib' """
			""" && """
#		""" install -d '%(prefix)s/lib' && """
#		""" install -m644 '$(1)/libbz2.a' '%(prefix)s/lib/' && """
#		""" install -d '%(prefix)s/include' && """
#		""" install -m644 '$(1)/bzlib.h' '%(prefix)s/include/' && """
		""" echo Done. """
	)

package( "boost",
	uri = "http://www.boost.org/",
	checkVersion =
		""" wget -q -O- 'http://sourceforge.net/projects/boost/files/boost/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" grep -v beta | """
		""" head -1 | """
		""" sed 's,\\.,_,g'""",
	tarballName = "%(name)s_%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/sourceforge/boost/%(tarball)s",
	srcdir = "%(name)s_%(version)s",
	deps = "zlib bzip2 expat",
	buildCommand =
		""" cd %(srcdir)s && """
		""" echo 'using gcc : : %(target)s-g++ : <rc>%(target)s-windres <archiver>%(target)s-ar ;' > 'user-config.jam' && """
		""" sed -i 's,<target-os>windows : lib ;,<target-os>windows : a ;,' '%(srcdir)s/tools/build/v2/tools/types/lib.jam' && """
#		""" cd 'tools/jam/src' && ./build.sh && """
		""" cd %(srcdir)s && """
#		""" tools/jam/src/bin.*/bjam """
		""" bjam """
			""" --ignore-site-config """
			""" --user-config=user-config.jam """
			""" target-os=windows """
			""" threading=multi """
			""" threadapi=win32 """
			""" --layout=tagged """
			""" --without-mpi """
			""" --without-python """
			""" --prefix='%(prefix)s' """
			""" --exec-prefix='%(prefix)s/bin' """
			""" --libdir='%(prefix)s/lib' """
			""" --includedir='%(prefix)s/include' """
			""" -sZLIB_INCLUDE='%(prefix)s/include' """
			""" -sZLIB_LIBPATH='%(prefix)s/lib' """
			""" -sEXPAT_INCLUDE='%(prefix)s/include' """
			""" -sEXPAT_LIBPATH='%(prefix)s/lib' """
			""" stage install """
			""" && """
		""" echo Package %(name)s done."""
	)


package( "dlfcn-win32",
	uri = "http://code.google.com/p/dlfcn-win32/",
	checkVersion =
		""" wget -q -O- 'http://code.google.com/p/dlfcn-win32/downloads/list' | """
		""" sed -n 's,.*dlfcn-win32-\(r[0-9][r.0-9]*\)\.tar.bz2.*,\\1,p' | """
		""" sort -g | tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.bz2",
	downloadUri = "http://dlfcn-win32.googlecode.com/files/%(tarball)s",
	buildCommand = ""
		""" cd %(srcdir)s && """
		""" sed -i '$aecho Done\\n' configure &&  """
		""" ./configure """
			""" --prefix='%(prefix)s' """
			""" --cross-prefix='%(target)s-' """
			""" --incdir='%(prefix)s/include' """
			""" --libdir='%(prefix)s/lib' """
			""" --enable-shared """
			""" && """
		""" make && """
		""" make install """
	)

package( "ladspa-sdk",
	uri = "http://www.ladspa.org/",
	deps = "dlfcn-win32",
	checkVersion =
		""" wget -q -O- 'http://www.ladspa.org/download/' | """
		""" sed -n 's,.*ladspa_sdk_\([0-9][.0-9]*\)\.tgz.*,\\1,p' | """
		""" sort -n | """
		""" tail -1 """,
	tarballName = "ladspa_sdk_%(version)s.tgz",
	srcdir = "ladspa_sdk",
	downloadUri = "http://www.ladspa.org/download/%(tarball)s",
	buildCommand = 
		""" cd %(srcdir)s/src && """
		""" sed -i 's,-fPIC,,' makefile && """
		""" sed -i 's,\(mkdirhier\),mkdir -p,' makefile && """
		""" make install """
			""" CC=%(target)s-gcc """
			""" CPP=%(target)s-g++ """
			""" LD=%(target)s-g++  """
			""" PREFIX=%(prefix)s """
			""" INCLUDES:='-I. -I%(prefix)s/include' """
			""" LIBRARIES:='-lm -ldl -L%(prefix)s/lib' """
			# TODO: applyplugin depends on endian.h, not in mingw
			""" PROGRAMS="../bin/analyseplugin ../bin/listplugins" """
	)

package( "lv2core",
	uri = "http://lv2plug.in",
	deps = "",
	checkVersion = 
		""" wget -q -O- 'http://lv2plug.in/spec/?C=M;O=D' | """
		""" grep lv2core | """
		""" grep -v sig | """
		""" grep -v pre | """
		""" sed -n 's,.*>lv2core-\(.*\)\.tar.[^<]*<.*,\\1,p' | """
		""" head -n 1 """,
	tarballName = "%(name)s-%(version)s.tar.bz2",
	downloadUri = "http://lv2plug.in/spec/%(tarball)s",
	buildCommand = 
		""" cd %(srcdir)s && """
		""" python waf configure --prefix=%(prefix)s && """
		""" python waf && """
		""" python waf install && """
		""" echo Package %(name)s done."""
)

package( "directx",
	uri = "http://www.microsoft.com",
	checkVersion = "echo 8.0",
	tarballName = "dx80_mgw.zip",
	downloadUri = "http://alleg.sourceforge.net/files/%(tarball)s",
	srcdir = "dx80_mgw",
	buildCommand = 
		""" cd %(srcdir)s && """
		# binaries are already provided by mingw, missing the includes
		""" cp include/dinput.h include/dsound.h %(prefix)s/include && """
		""" echo Package %(name)s done."""
	)



package( "portaudio",
	uri = "http://www.portaudio.com",
	deps = "directx",
	checkVersion =
		""" wget -q -O- 'http://www.portaudio.com/download.html' | """
		""" sed -n 's,.*pa_stable_v\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "pa_stable_v%(version)s.tar.gz",
	downloadUri = "http://www.portaudio.com/archives/%(tarball)s",
	srcdir = "portaudio",
	buildCommand = 
		""" cd %(srcdir)s && """
		""" sed -i '57a#define bzero(b,len) (memset((b), 0, (len)), (void) 0)' test/patest_read_write_wire.c  && """
		""" autoconf && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" --with-winapi=directx """
			""" --with-dxdir=%(prefix)s """
			""" CFLAGS='-I%(prefix)s/include ' """ # to find dx headers
			""" && """
		""" make && """
		""" make install """
	)


package( "xerces-c",
	uri = "http://xerces.apache.org/xerces-c/",
	deps = "", # TODO
	checkVersion =
		""" wget -q -O- 'http://www.apache.org/dist/xerces/c/3/sources/?C=M;O=D' | """
		""" sed -n 's,.*<a href="xerces-c-\([0-9][^"]*\)\.tar.*,\\1,p' | """
		""" grep -v rc | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "%(apachemirror)s/xerces/c/3/sources/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
#		""" autoconf && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" && """
		""" make && """
		""" make install """
	)


package( "libsigc++",
	uri = "http://libsigc.sourceforge.net/",
	deps = "",
	checkVersion = gnomeCheckVersion("libsigc++2", "2.2"),
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://ftp.gnome.org/pub/GNOME/sources/%(name)s/2.2/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" && """
		""" make && """
		""" make install """
	)

package( "libxml2",
	uri = "http://xmlsoft.org/",
	deps = "",
	checkVersion = # gnomeCheckVersion("libxml2",".") but with a 'v' in the version :-P
		""" wget -q -O- 'http://git.gnome.org/browse/libxml2/refs/tags' | """
		""" grep '<a href=' | sed -n 's,.*<a[^>]*>v\([0-9][^<]*\)<.*,\\1,p' | """
		""" sort | """
		""" tail -1 """ ,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "ftp://xmlsoft.org/libxml2/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i 's,`uname`,MinGW,g' 'xml2-config.in' && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" --without-debug """
			""" --without-python """
			""" --without-threads """
			""" && """
		""" make && """
		""" make install """
	)


package( "jpeg",
	uri = "http://www.ijg.org/",
	checkVersion = 
		""" wget -q -O- 'http://www.ijg.org/' | """
		""" sed -n 's,.*jpegsrc\.v\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "jpegsrc.v%(version)s.tar.gz",
	downloadUri = "http://www.ijg.org/files/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure  --prefix='%(prefix)s' --host='%(target)s' """
			""" && """ 
		""" make install """
	)

package( "lcms",
	uri = "http://www.littlecms.com/",
	deps = "jpeg zlib", # TODO: tiff?
	checkVersion = 
		""" wget -q -O- 'http://sourceforge.net/projects/lcms/files/lcms/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	tarballName = "lcms%(majorversion)s-%(version)s.tar.gz",
	downloadUri = "%(sfmirror)s/project/lcms/lcms/%(minorversion)s/%(tarball)s",
	srcdir = "lcms%(majorversion)s-%(version)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure  --prefix='%(prefix)s' --host='%(target)s' """
			""" CPPFLAGS='-I%(prefix)s/include' """
			""" LDFLAGS='-L%(prefix)s/lib' """
			""" && """ 
		""" make install """
	)

package( "libmng",
	uri = "http://www.libmng.com",
	deps = "zlib jpeg",
	checkVersion = 
		""" wget -q -O- 'http://sourceforge.net/projects/libmng/files/libmng-devel/' | """
		""" sed -n 's,.*/\([0-9][^"]*\)/".*,\\1,p' | """
		""" head -1 """,
	tarballName = "%(name)s-%(version)s.tar.bz2",
	downloadUri = "%(sfmirror)s/project/libmng/libmng-devel/%(version)s/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
#		""" sed -i '/.*lcms\.h.*/s.lcms\.h,lcms2.h,p' libmng_types.h && """
		""" make install -f makefiles/makefile.mingwdll """
			""" CC='%(target)s-gcc' """
			""" DLLWRAP='%(target)s-dllwrap' """
			""" LD='%(target)s-ld' """
			""" AR='%(target)s-ar rc' """
			""" RANLIB='%(target)s-ranlib' """
			""" INSTALL_PREFIX='%(prefix)s/' """
			""" ZLIBINC="`pkg-config zlib --cflags`" """
			""" ZLIBLIB="`pkg-config zlib --libs`"  """
#			""" LCMSINC="`pkg-config lcms2 --cflags` -DMNG_INCLUDE_LCMS" """ # uses lcms1 not lcms2
#			""" LCMSLIB="`pkg-config lcms2 --libs`" """
	)

package( "glib",
	uri = "http://www.gtk.org",
	deps = "gettext libiconv zlib",
	checkVersion = gnomeCheckVersion("glib", "2.27"),
	pinnedVersion = "2.27.4",
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://ftp.gnome.org/pub/GNOME/sources/%(name)s/%(minorversion)s/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
#		""" aclocal && libtoolize --force && autoconf && """
		""" sed -i 's,cross_compiling=no,cross_compiling=yes,' 'configure' && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" CFLAGS='-I%(prefix)s/include' """
			""" LDFLAGS='-L%(prefix)s/lib' """
			""" CXX=%(target)s-g++ """
			""" --with-libiconv=gnu """
			""" && """
		# specific to avoid non-working documentation target
		""" make -C 'glib'    install && """
		""" make -C 'gmodule' install && """
		""" make -C 'gthread' install && """
		""" make -C 'gobject' install && """
		""" make -C 'gio'     install MISC_STUFF= && """
		""" make              install-pkgconfigDATA """ # crossmingevn also installed install-configexecincludeDATA but failed
	)

package( "glibmm",
	uri = "http://www.gtkmm.org/",
	deps = "libsigc++ glib",
	checkVersion = gnomeCheckVersion("glibmm", "2.24"),
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://ftp.gnome.org/pub/GNOME/sources/%(name)s/%(minorversion)s/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./autogen.sh  --prefix='%(prefix)s'  --host='%(target)s' """
			""" --disable-documentation """ # docs fails to build
			""" && """
		""" make && """
		""" make install """
	)

package( "libxml++",
	uri = "http://libxmlplusplus.sourceforge.net/",
	deps = "glibmm libxml2",
	checkVersion =
		""" wget -q -O- 'http://git.gnome.org/browse/libxml++/refs/tags' | """
		""" grep '<a href=' | sed -n 's,.*<a[^>]*>\([0-9][^<]*\)<.*,\\1,p' | """
		""" grep 2.32 | """ # downloadUri limits us
		""" sort | """
		""" tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://ftp.gnome.org/pub/GNOME/sources/libxml++/%(minorversion)s/libxml++-%(version)s.tar.gz",
	buildCommand =
		""" cd %(srcdir)s && """
#		""" autoconf && """
		""" ./configure  --prefix='%(prefix)s'  --host='%(target)s' """
			""" --disable-documentation """ # docs fails to build
			"""  """
			""" && """
		""" make && """
		""" make install """
	)

#############################################################################
# Warning: Not-working border
# Modules below this line are work in progress
#############################################################################

package( "qt",
	uri = "http://qt.nokia.com/",
	deps = "gcc zlib libpng jpeg libmng tiff giflib libodbc++ postgresql freetds openssl libgcrypt sqlite libiconv",
	checkVersion =
		""" wget -q -O- 'http://qt.gitorious.org/qt/qt/commits' | """
		""" grep '<li><a href="/qt/qt/commit/' | """
		""" sed -n 's,.*<a[^>]*>v\([0-9][^<-]*\)<.*,\\1,p' | """
		""" tail -1 """,
	tarballName = "%(name)s-everywhere-opensource-src-%(version)s.tar.gz",
	downloadUri = "http://get.qt.nokia.com/qt/source/%(tarball)s",
	srcdir = "%(name)s-everywhere-opensource-src-%(version)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i '/i686-pc-mingw32/s,i686-pc-mingw32,%(target)s,p' mkspecs/unsupported/win32-g++-cross/qmake.conf  && """
#		""" OPENSSL_LIBS="`pkg-config --libs-only-l openssl`" """
#		""" PSQL_LIBS="-lpq -lsecur32 `'$(TARGET)-pkg-config' --libs-only-l openssl` -lws2_32" """
#		""" PKG_CONFIG_LIBDIR='%(prefix)s/lib/pkgconfig' """ # did not solved anything
		""" ./configure """
			""" -opensource """
			""" -confirm-license """
			""" -fast """
			""" -xplatform unsupported/win32-g++-cross """
			""" -force-pkg-config """
			""" -release """
			""" -exceptions """
#			""" -static """
			""" -prefix '%(prefix)s' """
			""" -prefix-install """
			""" -script """
			""" -opengl desktop """
			""" -webkit """
#			""" -no-glib """
#			""" -no-gstreamer """
#			""" -no-phonon """
#			""" -no-phonon-backend """
			""" -accessibility """
			""" -no-reduce-exports """
			""" -no-rpath """
			""" -make libs """
			""" -nomake demos """
			""" -nomake docs """
			""" -nomake examples """
			""" -no-sql-sqlite """ # clam do not need sql dependencies
			""" -no-sql-odbc """
			""" -no-sql-psql """
			""" -no-sql-tds """
			""" -system-zlib """
			""" -system-libpng """
			""" -system-libjpeg """
			""" -system-libtiff """
			""" -system-libmng """
			""" -system-sqlite """
#			""" -dbus-linked """
#			""" -openssl-linked """
			""" -no-openssl """
#			""" -I'%(prefix)s/include' """
#			""" -L'%(prefix)s/lib' """
			""" -v """
			""" && """
		""" make install """
	)

# TODO: detect lv2
package("clam",
	uri = "http://clam-project.org",
	checkVersion = "echo 1.5",
	downloadUri = "",
	tarballName = "%(name)s-%(version)s.tar.gz",
	buildCommand =
#		""" svn co http://clam-project.org/clam/trunk/CLAM %(srcdir)s && """
		""" cd %(srcdir)s && """
		""" scons configure """
			""" prefix='%(prefix)s' """
			""" crossmingw=1 """
			""" sandbox_path='%(prefix)s/../'"""
			""" audio_backend=portaudio """
			""" xmlbackend=both """
			""" with_ladspa=1 """
			""" with_fftw3=1 """
			""" && """
		""" scons install && """
		""" echo Package %(name)s done."""
)



# See http://kampfwurst.net/python-mingw32/
# It seems to be no clear way to crosscompile python
# Mingw support seems broken
# TODO: http://bugs.python.org/issue1597850
# And crosscompilation seems to be also broken
# TODO: http://bugs.python.org/issue3871
package( "python",
	uri = "http://www.python.org",
	checkVersion =
		""" wget -O- -q 'http://www.python.org/download/releases/' | """
		""" sed -n 's,.*releases/\(2\.4[0-9.]*\).*,\\1,p' | """
		""" sort -g | """
		""" tail -1 """,
	srcdir = "Python-%(version)s",
	tarballName = "Python-%(version)s.tar.bz2",
	downloadUri = "http://www.python.org/ftp/python/%(version)s/%(tarball)s",
	buildCommand = 
		""" cd %(srcdir)s && """
		""" autoheader && aclocal && autoconf && """
# http://kampfwurst.net/python-mingw32/ defines this for configure
#		""" CC=%(target)s-gcc """
#		""" CXX=%(target)s-g++ """
#		""" AR=%(target)s-ar """
#		""" RANLIB=%(target)s-ranlib """
		""" ./configure --host=%(target)s --target=%(target)s --prefix=%(prefix)s  """
			""" --enable-unicode=ucs4 """
			""" --with-threads """
			""" --with-cxx-main=%(target)s-g++ """
			""" && """
		""" make && """
		""" make install """
#		""" python setup.py build --compiler=mingw32 """
	)


order = """
	pthread
	fftw
	libmad
	id3lib
	libogg
	libvorbis
	gettext
	libiconv
	flac
	libsndfile
	speex
	liblo
	zlib
	libpng
	cppunit
	bzip2
	boost
	dlfcn-win32
	ladspa-sdk
	lv2core
	directx
	portaudio
	xerces-c
	libsigc++
	libxml2
	jpeg
	lcms
	libmng
	glib
	glibmm
	libxml++
	qt
	python
""".split()


def hasOption(option) :
	if option not in sys.argv :
		return False
	sys.argv.remove(option)
	return True

def parameterOption(option) :
	if option not in sys.argv :
		return None
	optionIndex = sys.argv.index(option)
	value = sys.argv[optionIndex+1]
	del sys.argv[optionIndex:optionIndex+2]
	return value

if hasOption('--list') :
	print "Available packages: " + " ". join(packageDatabase.keys())
	sys.exit()

fromPackage = parameterOption("--from")
if fromPackage is not None :
	fromPackage in order or die("Argument to --from '%s' is not a package. Use --list to see the list of packages."%fromPackage)
	order = order[order.index(fromPackage):]

deps = hasOption('--deps')
skipDeploy = hasOption('--skip-deploy')

if len(sys.argv)<=1 :
#	buildAll()
	build(" ".join(order))
else :
	build(" ".join(sys.argv[1:]))

