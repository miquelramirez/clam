#!/usr/bin/python
import os
import glob
import sys

options = Options('options.cache', ARGUMENTS)
options.Add(PathOption('prefix', 'The prefix where the application will be installed', ''))
options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', ''))
options.Add(('qt_plugins_install_path', 'Path component (without the install prefix) where to install designer plugins (tipically /lib/qt4/plugins/designer)','/bin/designer'))
options.Add(BoolOption('verbose', 'Display the full command line instead a short command description', 'no') )

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files

def recursiveDirs(root) :
	return filter( (lambda a : a.rfind( ".svn")==-1 ),  [ a[0] for a in os.walk(root)]  )

env = Environment(ENV=os.environ, tools=['default'], options=options)
options.Save('options.cache', env)
Help(options.GenerateHelpText(env))

env.SConsignFile() # Single signature file

CLAMInstallDir = env['clam_prefix']
clam_sconstoolspath = os.path.join(CLAMInstallDir,'share','clam','sconstools')

env.Tool('qt4', toolpath=[clam_sconstoolspath])
env.Tool('clam', toolpath=[clam_sconstoolspath])
env.Tool('nsis', toolpath=[clam_sconstoolspath])
if sys.platform=='darwin' : env.Tool('bundle', toolpath=[clam_sconstoolspath])
env.Tool('dmg', toolpath=[clam_sconstoolspath])
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
	] , CLAMInstallDir)

env.EnableQt4Modules([
	'QtCore',
	'QtGui',
	'QtOpenGL',
#	'QtSql',
#	'QtNetwork',
#	'QtTest',
	'QtXml',
	'QtSvg',
	'QtUiTools',
	'QtDesigner',
#	'Qt3Support',
	], debug=False)

mainSources = {
	'NetworkEditor' : os.path.join('src','main.cxx'),
	'Prototyper' : os.path.join('src','prototyper','main.cxx'),
	'OfflinePlayer' : os.path.join('src','OfflinePlayerMain.cxx'),
	'TestAudioDatabaseReader' : os.path.join('src','ebowSynthesizer','TestAudioDatabaseReader.cxx'), #TODO provisinal. Will move to CLAM/example
	'ebowSynthesizer' : os.path.join('src','ebowSynthesizer','ebowSynthesizer.cxx'), #TODO provisinal. Will move to CLAM/example
}

sourcePaths = [
	os.path.join('src'),
	os.path.join('src','generated'),
	os.path.join('src','processing'),
	os.path.join('src','processing','SampleBySample'),
	os.path.join('src','prototyper'),
	os.path.join('src','monitors'),
	os.path.join('src','monitors','generated'),
	os.path.join('src','clamWidgetsPlugins'),
	os.path.join('src','clamWidgetsPlugins','generated'),
	os.path.join('src','ebowSynthesizer'),
]
extraPaths = [
	CLAMInstallDir+'/include',
	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes
]
includePaths = sourcePaths + extraPaths

sources = scanFiles('*.cxx', sourcePaths)
sources = filter( (lambda a : a.rfind( "moc_")==-1 ),  sources )
sources = filter( (lambda a : a.rfind( "qrc_")==-1 ),  sources )
sources = dict.fromkeys(sources).keys()
for mainSource in mainSources.values() :
	sources.remove(mainSource)

qrcfiles = scanFiles("*.qrc", sourcePaths)
if qrcfiles : sources += env.Qrc(source=qrcfiles)

uifiles = scanFiles("*.ui", sourcePaths)
if uifiles: uiheaders = env.Uic4(source=uifiles)

if sys.platform=="win32" :
	sources += env.RES(source=["resources/NetworkEditor.rc"])

if sys.platform=='linux2' :
	# TODO: This should not be hardcoded neither prefix (because package install)
	env.Append(CPPFLAGS='-DDATA_EXAMPLES_PATH="\\"/usr/share/networkeditor/example-data\\""')

if sys.platform=='linux2' :
	env.Append( CCFLAGS=['-g','-O3','-Wall'] )
#	env.Append( LINKFLAGS=['-rdynamic'] ) # TODO: Is it needed?
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
pluginsources.append(os.path.join('src','monitors','generated','moc_Oscilloscope.cxx'))
pluginsources.append(os.path.join('src','monitors','generated','moc_SpectrumView.cxx'))
pluginsources.append(os.path.join('src','monitors','generated','moc_PeakView.cxx'))
pluginsources.append(os.path.join('src','monitors','generated','moc_PolarChromaPeaks.cxx'))
pluginsources.append(os.path.join('src','monitors','PixmapWidgets.cxx'))
pluginsources.append(os.path.join('src','monitors','Vumeter.cxx'))
pluginsources.append(os.path.join('src','monitors','ChordRanking.cxx'))
pluginsources.append(os.path.join('src','monitors','Tonnetz.cxx'))
pluginsources.append(os.path.join('src','monitors','KeySpace.cxx'))
pluginsources.append(os.path.join('src','monitors','LPModelView.cxx'))
pluginsources.append(os.path.join('src','monitors','MelSpectrumView.cxx'))
pluginsources.append(os.path.join('src','monitors','MelCepstrumView.cxx'))
pluginsources.append(os.path.join('src','monitors','BarGraph.cxx'))
pluginsources.append(os.path.join('src','monitors','QSynthKnob.cxx'))
pluginsources.append(os.path.join('src','ControlSurfaceWidget.cxx'))
pluginsources.append(os.path.join('src','processing','ControlSurface.cxx'))
env.Append(CPPPATH=includePaths+plugindirs)

#commonObjects = env.StaticLibrary(target="networkeditor", source=sources)
#programs = [ env.Program(target=program, source = [main, commonObjects])
#	for program, main in mainSources.items()]

programs = [ env.Program(target=program, source = [main] + sources)
	for program, main in mainSources.items()]


pluginDefines=['-DQT_PLUGIN','-DQT_NO_DEBUG','-DQT_CORE_LIB','-DQT_GUI_LIB','-DQT_OPENGL_LIB','-DQT_XML_LIB','-DQDESIGNER_EXPORT_WIDGETS','-D_REENTRANT']
env.AppendUnique(CPPFLAGS=pluginDefines)

#env.AppendUnique(CPPFLAGS=['-fPIC']) # qtPlugin examples were compiled with this option
# TODO: Move this to the qt4 tool
if os.getenv('QTDIR') :
	env.AppendUnique(QT4_MOCFROMHFLAGS=['-I'+os.path.join(env['QTDIR'],'include')])
	env.AppendUnique(QT4_MOCFROMCXXFLAGS=['-I'+os.path.join(env['QTDIR'],'include')])
else:
	env.AppendUnique(QT4_MOCFROMHFLAGS=['-I/usr/include/qt4'])
	env.AppendUnique(QT4_MOCFROMCXXFLAGS=['-I/usr/include/qt4'])

if sys.platform == "win32":
	env.AppendUnique(LINKFLAGS='/OPT:NOREF')

qtplugin = env.SharedLibrary("CLAMWidgets", pluginsources + env.Qrc(qrcfiles))

manpages = [
	'resources/man/man1/NetworkEditor.1',
	'resources/man/man1/Prototyper.1',
	]

# Manual step: lupdate-qt4 *xx *ui -ts NetworkEditor_ca.ts
tsfiles = scanFiles("*.ts", ["src/i18n/"])
#env.NoClean(tsfiles) # TODO: this is not enough!! scan -c will delete ts files!!!
translatableSources = scanFiles('*.cxx', sourcePaths);
translatableSources+= scanFiles('*.hxx', sourcePaths);
translatableSources+= scanFiles('*.ui', sourcePaths);
translatableSources = filter( (lambda a : a.rfind( "generated/")==-1 ),  translatableSources )
translations = []
if len(tsfiles) :
#	tsNodes = env.Ts(target=tsfiles, source = translatableSources)
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

if sys.platform=='win32' :
	installTargets += [
		env.Install(
			env['prefix']+"/bin",
			os.path.join(env['QTDIR'],'lib',"Qt"+dll+"4.dll")
			) for dll in 'Core', 'Gui', 'OpenGL']
	env.Append(NSIS_OPTIONS=['/DVERSION=%s' % fullVersion ])
	env.Append(NSIS_OPTIONS=['/DQTDIR=$QTDIR'])
	externalsDllDir = os.environ['EXTERNALDLLDIR']
	env.Append(NSIS_OPTIONS=['/DEXTERNALDLLDIR=%s' % externalsDllDir ])
	# Get the visual studio runtimes path
	for vcRuntimeDir in os.environ['PATH'].split(";") :
		vcRuntimeDir = os.path.normpath(vcRuntimeDir)
		if os.access(os.path.join(vcRuntimeDir,"msvcr71.dll"),os.R_OK) :
			break
	env.Append(NSIS_OPTIONS=['/DVCRUNTIMEDIR=%s' % vcRuntimeDir ])
	win_packages = [env.Nsis( source='resources\\clam_networkeditor.nsi')]
	env.AddPreAction(win_packages, '%s\\changeExampleDataPath.py . ..' % clam_sconstoolspath)
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

