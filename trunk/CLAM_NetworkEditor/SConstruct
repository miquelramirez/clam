#!/usr/bin/python
import os
import glob
import sys

version='0.3.1-CVS-20060601-1'
options = Options('options.cache', ARGUMENTS)
options.Add(PathOption('install_prefix', 'The prefix where the networkeditor will be installed', ''))
options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', ''))
options.Add(PathOption('clam_sconstools', 'The path to the scons tools provided by clam', ''))
options.Add(BoolOption('release', 'Build CLAM NetworkEditor enabling compiler optimizations', 'no') )
options.Add(BoolOption('verbose', 'Display the full command line instead a short command description', 'no') )


def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(path+"/"+pattern)
	return files

def recursiveDirs(root) :
	return filter( (lambda a : a.rfind( "CVS")==-1 ),  [ a[0] for a in os.walk(root)]  )

os.environ['EXTERNALDLLDIR']='f:\\clam-external-libs\dlls'

env = Environment(ENV=os.environ, tools=['default','qt'], options=options)
options.Save('options.cache', env)
Help(options.GenerateHelpText(env))

env.SConsignFile() # Single signature file

if sys.platform=="linux2" :
	env['QT_LIB']='qt-mt'
elif sys.platform=="win32" :
	env['QT_LIB']='qt-mt322'
	
#env.Tool('qt4', toolpath=[env['clam_sconstools']])
env.Tool('clam', toolpath=[env['clam_sconstools']])
env.Tool('nsis', toolpath=[env['clam_sconstools']])

CLAMInstallDir = env['clam_prefix']

env['CXXFILESUFFIX'] = '.cxx'
env['QT4_UICDECLSUFFIX'] = '.hxx'
env['QT4_MOCHPREFIX'] = 'generated/moc_'
env['QT4_UICDECLPREFIX'] = 'generated/ui_'
env['QT4_QRCCXXPREFIX'] = 'generated/qrc_'
if not env['verbose']:
	env['CXXCOMSTR'] = '== Compiling $SOURCE'
	env['LINKCOMSTR'] = '== Linking $TARGET'
	env['SHLINKCOMSTR'] = '== Linking library $TARGET'
	env['QT4_RCCCOMSTR'] = '== Embeding resources $SOURCE'
	env['QT4_UICCOMSTR'] = '== Compiling interface $SOURCE'
	env['QT4_LRELEASECOMSTR'] = '== Compiling translation $TARGET'
	env['QT4_MOCFROMHCOMSTR'] = '== Generating metaobjects for $SOURCE'
	env['QT4_MOCFROMCXXCOMSTR'] = '== Generating metaobjects for $SOURCE'

env.EnableClamModules([
	'clam_core',
	'clam_audioio',
	'clam_processing',
	'clam_vmqt'
	] , CLAMInstallDir)

#env.EnableQt4Modules([
#	'QtCore',
#	'QtGui',
#	'QtOpenGL',
#	'QtSvg',
#	'QtSql',
#	'QtNetwork',
#	'QtTest',
#	'QtXml',
#	'QtSvg',
#	'Qt3Support',
#	])

mainSources = {
	'NetworkEditor' : os.path.join('src','main.cxx'),
	'Prototyper' : os.path.join('src','Prototyper','BlockingPrototyper.cxx'),
}
blacklist = [
	os.path.join('src','Prototyper','PAPrototyper.cxx'),
]
if sys.platform=='linux2' :
	mainSources.update({
		'Prototyper-jack' : os.path.join('src','Prototyper','JACKPrototyper.cxx'),
	})
else :
	blacklist += [
		os.path.join('src','Prototyper','JACKPrototyper.cxx'),
	]

sourcePaths = [
	os.path.join('src'),
	os.path.join('src','GUI'),
	os.path.join('src','GUI','Base'),
	os.path.join('src','GUI','Interface'),
	os.path.join('src','GUI','Qt_Presentations'),
	os.path.join('src','GUI','Qt_Presentations','ConcretePresentations'),
	os.path.join('src','Processings'),
	os.path.join('src','Processings','MIDI'),
	os.path.join('src','Visualization'),
	os.path.join('src','Prototyper'),
]
extraPaths = []
extraPaths += [
	CLAMInstallDir+'/include',
	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes
#	'/usr/include/qt4/', # quick KLUDGE for ubuntu
]
extraPaths += [
	os.path.join('SimacServicesClient'),
	os.path.join('SimacServicesClient','generated'),
	]
includePaths = sourcePaths + extraPaths

sources = scanFiles('*.cxx', sourcePaths)
if sys.platform == "win32" :
	sources = filter( (lambda a : a.rfind( "Ladspa")==-1 ),  sources )
sources = filter( (lambda a : a.rfind( "moc_")==-1 ),  sources )
sources = filter( (lambda a : a.rfind( "qrc_")==-1 ),  sources )
sources = filter( (lambda a : a.rfind( "uic_")==-1 ),  sources )
sources = dict.fromkeys(sources).keys()
for mainSource in mainSources.values() :
	sources.remove(mainSource)
for blacksheep in blacklist :
	sources.remove( blacksheep )

qrcfiles = scanFiles("*.qrc", sourcePaths)
if qrcfiles : sources += env.Qrc(source=qrcfiles)

uifiles = scanFiles("*.ui", sourcePaths)
if uifiles: env.Uic(source=uifiles)
sources += [ os.path.join( os.path.dirname(uiccpp),
		'uic_'+os.path.splitext(os.path.basename(uiccpp))[0]+'.cxx') for uiccpp in uifiles ]
sources += [ os.path.join( os.path.dirname(uiccpp),
		'moc_'+os.path.splitext(os.path.basename(uiccpp))[0]+'.cxx') for uiccpp in uifiles ]

if sys.platform=="win32" :
	sources += env.RES(source=["resources/NetworkEditor.rc"])

env.Append(LIBS=['qui'])
env.Append(CPPPATH=includePaths)
env.Append(CPPFLAGS='-DRESOURCES_BASE="\\"' + env['install_prefix'] + '/share/networkeditor\\""')
env.Append(CPPFLAGS=['-DFFTW_HEADER="<rfftw.h>"'])
if sys.platform=='win32' :
	env.Append(CPPFLAGS=['-D_USE_MATH_DEFINES']) # to have M_PI defined

if sys.platform=='linux2' :
	if env['release'] :
		env.Append( CCFLAGS=['-g','-O3','-fomit-frame-pointer','-Wall'] )
	else :
		env.Append( CCFLAGS=['-g', '-Wall'] )
#	env.Append( LINKFLAGS=['-rdynamic'] ) # TODO: Is it needed?


programs = []
for main in mainSources.items() :
	programs += [ env.Program(target=main[0], source = sources+[main[1]]) ]

qtplugin = env.SharedLibrary("CLAMWidgets",os.path.join('src','QtDesignerPlugins','CLAMWidgetsPlugin.cxx'), 
	CPPFLAGS=['-DQ_THREAD_S QT_PLUGIN','-DQT_SHARED','-DQT_THREAD_SUPPORT'])

manpages = [
	'resources/man/man1/NetworkEditor.1',
	]

# Manual step: lupdate-qt4 *xx *ui -ts NetworkEditor_ca.ts
tsfiles = scanFiles("*.ts", ["src/i18n/"])
env.Precious(tsfiles) # TODO: this is not enough!! scan -c will delete ts files!!!
translatableSources = scanFiles('*.cxx', sourcePaths);
translatableSources+= scanFiles('*.hxx', sourcePaths);
translatableSources+= scanFiles('*.ui', sourcePaths);
translatableSources = filter( (lambda a : a.rfind( "generated/")==-1 ),  translatableSources )
translations = []
if len(tsfiles) : 
#	tsNodes = env.Ts(target=tsfiles, source = translatableSources)
	translations = env.Qm(source = tsfiles)

examples = []
for ext in ['xml', 'pos', 'ui', 'wav', 'mp3', 'ogg']:
	examples += scanFiles('*.%s'%ext, ['example-data'])

installation = {
	'/bin' : programs,
	'/bin/designer': [qtplugin],
	'/share/man/man1' : manpages,
	'/share/networkeditor/i18n': translations,
	'/share/networkeditor/example-data': examples,
}

installTargets = [
	env.Install( env['install_prefix']+path, files ) for path, files in installation.items() ]

if sys.platform=='win32' : 
	"""
	installTargets += [
		env.Install(
			env['install_prefix']+"/bin", 
			os.path.join(env['QTDIR'],'lib',"Qt"+dll+"4.dll")
			) for dll in 'Core', 'Gui', 'OpenGL']
	"""
	env.Append(NSIS_OPTIONS=['/DVERSION=%s' % version ])
	env.Append(NSIS_OPTIONS=['/DQTDIR=$QTDIR'])
	externalsDllDir = os.environ['EXTERNALDLLDIR'] 
	env.Append(NSIS_OPTIONS=['/DEXTERNALDLLDIR=%s' % externalsDllDir ])
	# Get the visual studio runtimes path
	for vcRuntimeDir in os.environ['PATH'].split(";") :
		vcRuntimeDir = os.path.normpath(vcRuntimeDir)
		if os.access(os.path.join(vcRuntimeDir,"msvcr71.dll"),os.R_OK) :
			break
	env.Append(NSIS_OPTIONS=['/DVCRUNTIMEDIR=%s' % vcRuntimeDir ])
	win_packages = [env.Nsis( source='scons\\clam_networkeditor.nsi')]
	env.Alias('package', win_packages)

if sys.platform=='macosx' : # not really tested!!!
	env.Append(CPPFLAGS='-DRESOURCES_BASE="\\"NetworkEditor.app/Contents/Resources\\""')
	#Binaries: networkeditor + sample extractor
	env.AppendUnique( LINKFLAGS=['-dynamic','-bind_at_load'])

	#Resource installation in Mac application directory (binaries, xml metadata, icon, sound)
	installTargets = [
		env.Install( 'NetworkEditor.app/Contents/MacOS', programs ),
		env.Install( 'NetworkEditor.app/Contents', 'resources/Info.plist'),
		env.Install( 'NetworkEditor.app/Contents/Resources', 'resources/CLAM.icns'),
		]
	mac_packages = env.Dmg('notused', installTargets )
	env.Alias('package', mac_packages)

env.Alias('install', installTargets )

env.Default(programs+[qtplugin], translations)

