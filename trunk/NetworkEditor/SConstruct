#!/usr/bin/python
import os
import glob
import sys

options = Options('options.cache', ARGUMENTS)
options.Add(PathOption('prefix', 'The prefix where the application will be installed', ''))
options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', ''))
options.Add(BoolOption('release', 'Enabling compiler optimizations', 'no') )
options.Add(('qt_plugins_install_path', 'Path component (without the install prefix) where to install designer plugins (tipically /lib/qt4/plugins/designer)','/bin/designer'))
options.Add(BoolOption('verbose', 'Display the full command line instead a short command description', 'no') )
options.Add(PathOption('external_dll_path', '(Windows only) The place where the NSIS packager takes the installed DLL from', '.'))
if sys.platform=="linux2" :
	options.Add(BoolOption('crossmingw', 'Using MinGW crosscompiler mode', 'no') )

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files

def recursiveDirs(root) :
	return filter( (lambda a : a.rfind( ".svn")==-1 ),  [ a[0] for a in os.walk(root)]  )

def unique(list) :
	return dict.fromkeys(list).keys()

toolchain='default'
if sys.platform == 'win32': toolchain = 'mingw'
env = Environment(ENV=os.environ, tools=[toolchain], options=options)
options.Save('options.cache', env)
Help(options.GenerateHelpText(env))

env.SConsignFile() # Single signature file

crosscompiling = env.has_key("crossmingw") and env["crossmingw"]
isWindowsPlatform = sys.platform=='win32' or crosscompiling
isLinuxPlatform = sys.platform=='linux2' and not crosscompiling
isDarwinPlatform = sys.platform=='darwin'

CLAMInstallDir = env['clam_prefix']
clam_sconstoolspath = os.path.join(CLAMInstallDir,'share','clam','sconstools')

env.Tool('qt4', toolpath=[clam_sconstoolspath])
env.Tool('clam', toolpath=[clam_sconstoolspath])
env.Tool('nsis', toolpath=[clam_sconstoolspath])
if sys.platform=='darwin' : env.Tool('bundle', toolpath=[clam_sconstoolspath])
env.Tool('dmg', toolpath=[clam_sconstoolspath])
if crosscompiling :
	env.Tool('crossmingw', toolpath=[clam_sconstoolspath])
sys.path.append(clam_sconstoolspath)
import versionInfo
version, fullVersion = versionInfo.versionFromLocalInfo("NetworkEditor")
print "Version: ", version
print "Package version: ", fullVersion
versionInfo.generateVersionSources(os.path.join('src','NetworkEditorVersion'), "NetworkEditor", fullVersion)


env['CXXFILESUFFIX'] = '.cxx'
env['QT4_UICDECLSUFFIX'] = '.hxx'
env['QT4_MOCHPREFIX'] = os.path.join('generated','moc_')
env['QT4_UICDECLPREFIX'] = os.path.join('generated','uic_')
env['QT4_QRCCXXPREFIX'] = os.path.join('generated','qrc_')
if not env['verbose']:
	env['CXXCOMSTR'] = '== Compiling $SOURCE'
	env['SHCXXCOMSTR'] = '== Compiling shared $SOURCE'
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
	], CLAMInstallDir)

env.EnableQt4Modules([
	'QtCore',
	'QtGui',
	'QtOpenGL',
	'QtXml',
	'QtSvg',
	'QtUiTools',
	'QtDesigner',
	],
	debug=False,
	crosscompiling=crosscompiling,
	)

mainSources = {
	'NetworkEditor' : os.path.join('src','main.cxx'),
	'Prototyper' : os.path.join('src','prototyper','main.cxx'),
	'OfflinePlayer' : os.path.join('src','OfflinePlayerMain.cxx'),
}

sourcePaths = [
	os.path.join('src'),
	os.path.join('src','jack'),
	os.path.join('src','generated'),
	os.path.join('src','processing'),
	os.path.join('src','processing','SampleBySample'),
	os.path.join('src','prototyper'),
	os.path.join('src','monitors'),
	os.path.join('src','monitors','generated'),
	os.path.join('src','widgets'),
	os.path.join('src','widgets','generated'),
	os.path.join('src','clamWidgetsPlugins'),
	os.path.join('src','clamWidgetsPlugins','generated'),
]
extraPaths = [
	CLAMInstallDir+'/include',
	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes
]
includePaths = sourcePaths + extraPaths

sources = scanFiles('*.cxx', sourcePaths)
sources = filter( (lambda a : a.rfind( "moc_")==-1 ),  sources )
sources = filter( (lambda a : a.rfind( "qrc_")==-1 ),  sources )
sources = unique(sources)
for mainSource in mainSources.values() :
	sources.remove(mainSource)

qrcfiles = scanFiles("*.qrc", sourcePaths)
if qrcfiles : sources += env.Qrc(source=qrcfiles)

uifiles = scanFiles("*.ui", sourcePaths)
if uifiles: uiheaders = env.Uic4(source=uifiles)

if isWindowsPlatform :
	sources += env.RES(source=["resources/NetworkEditor.rc"])

if isLinuxPlatform :
	env.AppendUnique(LIBS=['GLU'])
	# TODO: This should not be hardcoded neither prefix (because package install)
	env.Append(CPPFLAGS='-DDATA_EXAMPLES_PATH="\\"/usr/share/networkeditor/example-data\\""')

if env['release'] :
	env.Append( CCFLAGS=['-g','-O3','-fomit-frame-pointer','-Wall'] )
else :
	env.Append( CCFLAGS=['-g','-O3','-Wall'] )




testsources = scanFiles('*.cxx', ['test'])
plugindirs = [
	os.path.join('src','clamWidgetsPlugin'),
	os.path.join('src','clamWidgetsPlugin','generated'),
]
pluginsources = scanFiles('*.cxx', plugindirs)
pluginsources = filter( (lambda a : a.rfind( "moc_")==-1 ),  pluginsources )
pluginsources = filter( (lambda a : a.rfind( "qrc_")==-1 ),  pluginsources )
pluginsources = dict.fromkeys(pluginsources).keys()
# //TODO: review this
pluginsources.append(os.path.join('src','widgets','SpectrumView.cxx'))
pluginsources.append(os.path.join('src','widgets','PeakView.cxx'))
pluginsources.append(os.path.join('src','widgets','PolarChromaPeaks.cxx'))
pluginsources.append(os.path.join('src','widgets','Oscilloscope.cxx'))
pluginsources.append(os.path.join('src','widgets','PixmapWidgets.cxx'))
pluginsources.append(os.path.join('src','widgets','Vumeter.cxx'))
pluginsources.append(os.path.join('src','widgets','ChordRanking.cxx'))
pluginsources.append(os.path.join('src','widgets','Tonnetz.cxx'))
pluginsources.append(os.path.join('src','widgets','KeySpace.cxx'))
pluginsources.append(os.path.join('src','widgets','LPModelView.cxx'))
pluginsources.append(os.path.join('src','widgets','MelSpectrumView.cxx'))
pluginsources.append(os.path.join('src','widgets','MelCepstrumView.cxx'))
pluginsources.append(os.path.join('src','widgets','BarGraph.cxx'))
pluginsources.append(os.path.join('src','widgets','QSynthKnob.cxx'))
pluginsources.append(os.path.join('src','widgets','Spectrogram.cxx'))
pluginsources.append(os.path.join('src','ControlSurfaceWidget.cxx'))
pluginsources.append(os.path.join('src','widgets','QFirstPerson.cxx'))
pluginsources.append(os.path.join('src','processing','ControlSurface.cxx'))
pluginsources.append(os.path.join('src','processing','ProgressControl.cxx'))
env.Append(CPPPATH=includePaths+plugindirs)

"""
widgetsources=scanFiles('*.cxx', [os.path.join('src', 'widgets')])
print widgetsources
widgetsources=filter( (lambda a : a.rfind( "moc_")==-1 ),  widgetsources )
widgetsources=filter( (lambda a : a.rfind( "qrc_")==-1 ),  widgetsources )
print widgetsources
"""

#commonObjects = env.StaticLibrary(target="networkeditor", source=sources)
#programs = [ env.Program(target=program, source = [main, commonObjects])
#	for program, main in mainSources.items()]

programs = [ env.Program(target=program, source = [main] + sources)
	for program, main in mainSources.items()]


pluginDefines=['-DQT_PLUGIN','-DQT_NO_DEBUG','-DQT_CORE_LIB','-DQT_GUI_LIB','-DQT_OPENGL_LIB','-DQT_XML_LIB','-DQDESIGNER_EXPORT_WIDGETS','-D_REENTRANT']
env.AppendUnique(CPPFLAGS=pluginDefines)

#env.AppendUnique(CPPFLAGS=['-fPIC']) # qtPlugin examples were compiled with this option

qtplugin = env.SharedLibrary("CLAMWidgets", pluginsources + env.Qrc(qrcfiles))

manpages = [
	'resources/man/man1/NetworkEditor.1',
	'resources/man/man1/Prototyper.1',
	]

tsfiles = scanFiles("*.ts", ["src/i18n/"])
translatableSources = scanFiles('*.cxx', sourcePaths);
translatableSources+= scanFiles('*.hxx', sourcePaths);
translatableSources+= scanFiles('*.ui', sourcePaths);
translatableSources = filter( (lambda a : a.rfind( "generated/")==-1 ),  translatableSources )
translations = []
if len(tsfiles) :
	# Manual step: lupdate-qt4 *xx *ui -ts NetworkEditor_ca.ts
	#tsNodes = env.Ts(target=tsfiles, source = translatableSources)
	# TODO: move those settings to the qt4 tool
	#env.Precious(tsNodes) # Do not remove it until really regenerated
	#env.NoClean(tsNodes) # They are not just generated but also user edited
	translations = env.Qm(source = tsfiles)

qtpluginsInstallationPath = env['qt_plugins_install_path']
examples = []
for ext in ['clamnetwork', 'pos', 'ui', 'wav', 'mp3', 'ogg']:
	examples += scanFiles('*.%s'%ext, ['example-data'])

menuEntries = [
	'resources/Prototyper.desktop',
	'resources/NetworkEditor.desktop',
]
mimeEntries = [
	'resources/clam-network.xml',
]
kdeMimeEntries = [
	'resources/vnd.clam.network.desktop'
]

installation = {
	'/bin' : programs,
	qtpluginsInstallationPath : [qtplugin],
	'/share/applications': menuEntries,
	'/share/mime/packages': mimeEntries,
	'/share/mimelnk/application': kdeMimeEntries,
	'/share/man/man1' : manpages,
	'/share/networkeditor': ['resources/NetworkEditor-icon.svg'],
	'/share/networkeditor/i18n': translations,
	'/share/networkeditor/example-data': examples,
}

installTargets = [
	env.Install( env['prefix']+path, files ) for path, files in installation.items() ]

if isWindowsPlatform : 
	def absolutePosixPathToWine(dir) :
		return 'z:'+'\\\\'.join(dir.split('/'))

	env.Append(LIBS=['glu32'])
	winqtdir=env['QTDIR']
	if crosscompiling : env['NSIS_MAKENSIS'] = 'wine ~/.wine/dosdevices/c:/Program\ Files/NSIS/makensis'
	if crosscompiling : winqtdir = absolutePosixPathToWine(winqtdir)
	externalDllPath = env['external_dll_path']
	if crosscompiling : externalDllPath = absolutePosixPathToWine(externalDllPath)
	winclampath = CLAMInstallDir
	if crosscompiling : winclampath = absolutePosixPathToWine(winclampath)
	if crosscompiling :
		env.AddPostAction(programs, env.Action(["i586-mingw32msvc-strip $TARGET"], "== Stripping $SOURCE"))
	installTargets += [
		env.Install(
			env['prefix']+"/bin",
			os.path.join(env['QTDIR'],'bin',"Qt"+dll+"4.dll")
			) for dll in 'Core', 'Gui', 'OpenGL']
	env.Append(NSIS_OPTIONS=['/DVERSION=%s' % fullVersion ])
	env.Append(NSIS_OPTIONS=['/DQTDIR=%s'%winqtdir ])
	env.Append(NSIS_OPTIONS=['/DEXTERNALDLLDIR=%s' % externalDllPath ])
	env.Append(NSIS_OPTIONS=['/DCLAMINSTALLDIR=%s' % winclampath ])
	# Get the visual studio runtimes path
	for vcRuntimeDir in os.environ['PATH'].split(";") :
		vcRuntimeDir = os.path.normpath(vcRuntimeDir)
		if os.access(os.path.join(vcRuntimeDir,"msvcr71.dll"),os.R_OK) :
			break
	env.Append(NSIS_OPTIONS=['/DVCRUNTIMEDIR=%s' % vcRuntimeDir ])
	win_packages = [env.Nsis( source='resources/installer.nsi')]
	env.AddPreAction(win_packages, os.path.join(clam_sconstoolspath,'changeExampleDataPath.py')+' . ..' )
	env.Alias('package', win_packages)

if sys.platform=='darwin' :
	mac_networkeditor_bundle = env.Bundle(
		BUNDLE_NAME='NetworkEditor',
		BUNDLE_BINARIES=["NetworkEditor"],
		BUNDLE_RESOURCEDIRS=["example-data"],
		BUNDLE_PLIST='resources/NetworkEditor-Info.plist',
		BUNDLE_ICON='resources/CLAM.icns',
	 )
	mac_prototyper_bundle = env.Bundle(
		BUNDLE_NAME='Prototyper',
		BUNDLE_BINARIES=["Prototyper"],
		BUNDLE_PLUGINS=["libCLAMWidgets.dylib"],
		BUNDLE_RESOURCEDIRS=[],
		BUNDLE_PLIST='resources/Prototyper-Info.plist',
		BUNDLE_ICON='resources/Prototyper.icns',
	 )
	mac_designer_bundle = env.Bundle(
		BUNDLE_NAME='QtDesigner',
		BUNDLE_BINARIES=["$QTDIR/bin/Designer.app/Contents/MacOS/Designer"],
		BUNDLE_PLUGINS=[
			"libCLAMWidgets.dylib",
			"$QTDIR/plugins/designer/libarthurplugin.dylib",
		],
		BUNDLE_RESOURCEDIRS=[],
		BUNDLE_PLIST='resources/QtDesigner-Info.plist',
		BUNDLE_ICON='resources/QtDesigner.icns',
	 )
	# Kludge to fix the plugins location for Designer
	env.AddPostAction( mac_designer_bundle, env.Action( [
		'mv QtDesigner.app/Contents/MacOS/Designer QtDesigner.app/Contents/MacOS/QtDesigner',
		'mkdir QtDesigner.app/Contents/MacOS/designer',
		'mv QtDesigner.app/Contents/PlugIns/* QtDesigner.app/Contents/MacOS/designer',
	]))
	env.Alias('bundle', [mac_networkeditor_bundle, mac_prototyper_bundle, mac_designer_bundle])

	#TODO mac_bundle should be dependency of Dmg:
	arch = os.popen("uname -p").read().strip()
	mac_packages = env.Dmg('CLAM_NetworkEditor-%s-%s.dmg'% (fullVersion, arch), [
		env.Dir('NetworkEditor.app/'),
		env.Dir('Prototyper.app'),
		env.Dir('QtDesigner.app'),
	] )
	env.Alias('package', mac_packages)

env.Alias('install', installTargets )

env.Default(programs, translations, qtplugin)

