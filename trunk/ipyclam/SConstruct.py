#! /usr/bin/python

import os, glob, sys
libraryName='ipyclam.so'
print 'Building', libraryName
options = Variables('options.cache', ARGUMENTS)
options.Add(PathVariable('clam_prefix', 'The prefix where CLAM was installed', ''))
options.Add(PathVariable('prefix', 'Installation prefix (normally /usr, by default this is clam_prefix)', '', validator=PathVariable.PathAccept))
options.Add(BoolVariable('crossmingw', 'Using MinGW crosscompiler mode', 'no') )

toolChain = 'default'
if sys.platform == 'win32': toolChain = 'mingw'
env = Environment(ENV=os.environ, tools=[toolChain], options=options)
options.Save('options.cache', env) # do not save options in NetworkEditor generated plugins
Help(options.GenerateHelpText(env))
env.SConsignFile() # Single signature file

CLAMInstallDir = env['clam_prefix']
InstallDir = env['prefix'] or env['clam_prefix']
clam_sconstoolspath = os.path.join(CLAMInstallDir,'share','clam','sconstools')
if env['crossmingw'] :
	env.Tool('crossmingw', toolpath=[clam_sconstoolspath])
env.Tool('clam', toolpath=[clam_sconstoolspath])
env.EnableClamModules([
	'clam_core',
	'clam_audioio',
	'clam_processing',
	] , CLAMInstallDir)

env.ParseConfig("python-config --cflags")
env.ParseConfig("python-config --ldflags")


sources = ['ipyclam.cxx']
extraPaths = [
	CLAMInstallDir+'/include',
	CLAMInstallDir+'/include/CLAM', # KLUDGE to keep old style includes
]
env.Append(CPPPATH=extraPaths)

env.Append( CCFLAGS=['-g','-O3','-Wall'] )
env.Depends( libraryName, glob.glob("../example-data/*.clamnetwork"))
libraries = [
	env.SharedLibrary(target=libraryName, source = sources, SHLIBPREFIX=''),
	]
install = env.Install(os.path.join(InstallDir,'lib','python'), libraries)
env.Alias('install', install)
env.Default(libraries)

