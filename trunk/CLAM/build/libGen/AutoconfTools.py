import os, re

getHashes = re.compile( "\$\$(\w+)[{\[]\$\$(\w+)[}\]]" )
getVars = re.compile( "\$\$(\w+)" )

def parse_acv_file( filename, **args ) :
    args['filename'] = filename

    handle = file( filename, "r" )
    fragment = handle.read()
    handle.close()

    #Variable substitution
    # 1 - Hash variable search and substitution
    hashVariables = getHashes.findall( fragment )

    for hashname, key in hashVariables :
        fragment = re.sub( "\$\$%s[{\[]\$\$%s[}\]]"%(hashname,key), args[hashname], fragment )

    variables = getVars.findall( fragment )
    for varname in variables :
        fragment = re.sub( "\$\$%s(?=\W)"%varname, args[varname], fragment )

    return fragment

def ac_check_feature_enabled( featName, defaultState="yes",
                              effectsEnable="", effectsDisable="" ) :
    eFeatName = re.sub( "-", "_", featName )
    return parse_acv_file( "acv/check_feature_enable.acv",
                           feature=featName,
                           feature_esc=eFeatName,
                           uc_feature_esc=eFeatName.upper(),
                           default=defaultState,
                           effectsEnabled=effectsEnable,
                           effectsDisabled=effectsDisable )

def ac_disabled_feature_warning( featName ) :
    eFeatName = re.sub( "-", "_", featName )
    return parse_acv_file( "acv/disabled_feature_warning.acv",
                           feature=featName,
                           feature_esc=eFeatName,
                           uc_feature = featName.upper() )

def ac_check_package_enabled( featName, defaultState="yes" ) :
    eFeatName = re.sub( "-", "_", featName )
    return parse_acv_file( "acv/check_package_enabled.acv",
                           feature=featName,
                           feature_esc=eFeatName,
                           uc_feature_esc=eFeatName.upper(),
                           default=defaultState )

def ac_disabled_package_warning( featName ) :
    eFeatName = re.sub( "-", "_", featName )
    return parse_acv_file( "acv/disabled_package_warning.acv",
                           feature=featName,
                           feature_esc=eFeatName,
                           uc_feature = featName.upper() )



def ac_sandbox_location( ) :    
    frag1 = ac_check_feature_enabled( 'sandbox' )

    return "\n".join( [ frag1, parse_acv_file("acv/sandbox_location.acv") ] )


def ac_pkg_config_availability() :
    return "\n".join( [ ac_check_feature_enabled('pkg-config'),
                        parse_acv_file( 'acv/pkg_config_availability.acv') ] )

def ac( *args ) :
    # generates a AC_args[0]( args[1], args[2], ...  ) directive
    if len( args ) == 0 :
        return ""

    funcName = "AC_%s"%args[0]

    parmList = []
    for parm in args[1:] :
        parmList.append( "[%s]"%parm )
    parmList = ",".join( parmList )

    return "%s(%s)"%(funcName,parmList)

class Feature:
    def __init__( self, name,  isDefault = True ) :
        self.name = name
        if isDefault :
            self.default = 'yes'
        else :
            self.default = 'no'
        self.effectsEnabled = []
        self.effectsDisabled = []

    def check( self ) :
        return ac_check_feature_enabled( self.name,
                                         self.default,
                                         self.getEffectsWhenEnabled(),
                                         self.getEffectsWhenDisabled() )

    def addEffectWhenEnabled( self, effectString ) :
        self.effectsEnabled.append( effectString )

    def addEffectWhenDisabled( self, effectString ) :
        self.effectsDisabled.append( effectString )

    def getEffectsWhenEnabled( self ) :
        return " ".join( self.effectsEnabled )

    def getEffectsWhenDisabled( self ) :
        return " ".join( self.effectsDisabled )


class Library:
    def __init__( self, name, language, isDefault = True ) :
        self.name = name
        self.language = language.upper()
        if not self.language == 'C' and not self.language=='C++' :
            raise TypeError, "Unrecognized language %s specified. Only 'C' and 'C++' are supported"%language
        self.sandboxName = ""
        self.headers = ""
        self.libs = ""
        self.other_libs = ""
        self.function = ""
        self.source = """
int main( void )
{
    return 0;
}
"""
        if isDefault : 
            self.default = 'yes'
        else :
            self.default = 'no'
    
    def sandbox_find( self ) :
        eFeatName = re.sub( "-","_", self.name )
        if len( self.headers ) == 0 :
            raise TypeError, "No headers were specified for %s"%self.name
        return "\n".join( [ parse_acv_file( 'acv/feature_in_sandbox.acv',
                                            feature=self.name,
                                            feature_esc=eFeatName,
                                            headers=self.headers,
                                            libs=self.libs,
                                            other_libs=self.other_libs,
                                            source=self.source,
                                            sandbox=self.sandboxName) ] )
    def standard_find( self ) :
        eFeatName = re.sub( "-", "_", self.name )
        if len( self.headers ) == 0 :
            raise TypeError, "No headers was specified for %s"%self.name
        if len( self.libs ) == 0 :
            raise TypeError, "No libs was specified for %s"%self.name
        if len( self.function ) == 0 :
            raise TypeError, "No function was specified for %s"%self.name
        return parse_acv_file( "acv/standard_find.acv",
                               feature=self.name,
                               feature_esc=eFeatName,
                               headers=self.headers,
                               libs=self.libs,
                               other_libs=self.other_libs,
                               function=self.function,
                               source=self.source)

    def pkg_config_find( self ) :
        eFeatName = re.sub( "-", "_", self.name )
        return "\n".join( [ parse_acv_file( 'acv/pkg_config_find.acv',
                                            feature=self.name,                                           
                                            feature_esc=eFeatName) ] )

    def find( self ):
        return "\n".join( [ ac_check_package_enabled( self.name, self.default ),
                            self.sandbox_find(),
                            self.standard_find(),
                            self.pkg_config_find() ] )

    def test( self ):
        eFeatName = re.sub( "-","_", self.name)
        return parse_acv_file( 'acv/default_test.acv',
                               feature=self.name,
                               feature_esc=eFeatName,
                               source=self.source)

    def introspect( self ):
        eFeatName = re.sub( "-","_", self.name)
        out = list()

        out.append("AC_MSG_NOTICE([==========%s detection results=============])"%self.name)
        out.append("AC_MSG_NOTICE([found_%s = $found_%s])"%(eFeatName,eFeatName) )
        out.append("AC_MSG_NOTICE([include_dirs_%s = $include_dirs_%s])"%(eFeatName,eFeatName))
        out.append("AC_MSG_NOTICE([lib_dirs_%s = $lib_dirs_%s])"%(eFeatName,eFeatName))
        out.append("AC_MSG_NOTICE([libs_%s = $libs_%s])"%(eFeatName,eFeatName))
        out.append('include_dirs="$include_dirs $include_dirs_%s"'%eFeatName )        
        out.append('lib_dirs="$lib_dirs $lib_dirs_%s"'%eFeatName)
        out.append('libs="$libs $libs_%s"'%eFeatName)
        equals = '='*len(self.name)
        out.append("AC_MSG_NOTICE([==========%s===============================])"% equals)

        return "\n".join( out )
                                          
        

class AutoconfScript :
    # Autoconf script container
    def __init__( self, packageName, packageVersion, bugReporting = "clam@iua.upf.es" ) :
        self.packageName = packageName
        self.packageVersion = packageVersion
        self.bugReporting = bugReporting
        self.scriptLines = []
        self.featureList = []
        self.librariesList = []
        self.defaults = None
        self.descriptionText = ""
        self.packageDeps = []
        self.preinclude = ""

    def dependsOn( self, name ) :
        self.packageDeps.append( name )

    def __lshift__( self, line ) :
        # Inserts a new line in the script
        try:
            # EAFAP type checking
            line.isdigit()
            self.scriptLines.append( line )
        except AttributeError :
            self.scriptLines.extend( line )

    def addFeature( self, feature ) :
        self.featureList.append( feature )

    def addLibrary( self, library ) :
        self.librariesList.append( library )

    def fillLines( self ) :
        self.scriptLines = []
        self << ac( 'INIT', '%s'%self.packageName, '%s'%self.packageVersion, '%s'%self.bugReporting )
        self << ac( 'PROG_CC' )
        self << ac( 'PROG_CPP' )
        self << ac( 'PROG_CXX' )
        self << ac( 'PROG_CXXCPP')
        self << ac( 'SUBST', 'include_dirs')
        self << ac( 'SUBST', 'lib_dirs')
        self << ac( 'SUBST', 'libs')
        self << 'include_dirs=""\n'
        self << 'lib_dirs=""\n'
        self << 'libs=""\n'
        self << ac( 'SUBST','PACKAGE_DEFINES')
        self << 'PACKAGE_DEFINES=""\n'
        self << ac( 'SUBST','PACKAGE_PRE_INCLUDE')
        self << 'PACKAGE_PRE_INCLUDE="%s"\n'%self.preinclude
        self << ac( 'SUBST','PACKAGE_DESCRIPTION')
        self << 'PACKAGE_DESCRIPTION="%s"\n'%self.descriptionText
        self << ac( 'SUBST','PACKAGE_DEPS')
        self << 'PACKAGE_DEPS="%s"'%",".join( self.packageDeps )
        # Check install
        self << ac( 'PROG_INSTALL')
        self << ac( 'PATH_PROG', 'LDDCONFIG','ldconfig','','$PATH:/usr/sbin:/sbin:/bin' )
        self << ac( 'SUBST', 'PATH_PROG' )
        self << parse_acv_file( 'acv/sed_presence.acv' )
        self << ac_sandbox_location()
        self << ac_pkg_config_availability()

        for feature in self.featureList :
            self << feature.check()
            self.defaults.addFeature( feature.name )

        self << ac( 'LANG_C' )
        for lib in self.librariesList :
            if lib.language == 'C' :
                self << lib.find()
                if len( lib.source ) > 0 :
                    self << lib.test()
                self << lib.introspect()
                self << ac_disabled_package_warning( lib.name )
                self.defaults.addLibrary( lib.name )

        self << ac( 'LANG_CPLUSPLUS' )
        for lib in self.librariesList:
            if lib.language == 'C++' :
                self << lib.find()
                if len( lib.source ) > 0 :
                    self << lib.test()
                self << lib.introspect()
                self << ac_disabled_package_warning( lib.name )
                self.defaults.addLibrary( lib.name )

        self << 'outpkgconf="$PACKAGE_NAME.pc"'
        self << ac('OUTPUT','defaults.cfg system-linux.cfg Makefile.rules $outpkgconf')
#        self << ac('OUTPUT','system-linux.cfg')
#        self << ac('OUTPUT','Makefile.rules' )


    def commitToFile( self, filename ) :
        # Commits the current container contents
        # to the specified file
        stream = file( "%s/configure.in"%filename, "w" )
        self.defaults = DefaultsFile( filename )
        self.fillLines()
        stream.write( "\n".join( self.scriptLines ) )
        stream.close()
        del self.defaults


    def copySupportFiles( self, path ) :
        os.system("cp system-common.cfg %s/"%path )
        os.system("cp system-linux.cfg.in %s/"%path )
        os.system("cp system-macosx.cfg.in %s/"%path )
        os.system("cp system.cfg %s/"%path )
        os.system("cp system-win.cfg %s/"%path )
        os.system("cp install-sh %s/"%path )
        os.system("cp Makefile.rules.in %s/"%path )
        os.system("cp CLAMlib.pc.in.tpl %s/%s.pc.in"%(path,self.packageName ))


class DefaultsFile :
    def __init__( self, folder ) :
        self.handle = file( "%s/defaults.cfg.in"%folder, "w" )
        self.handle.write( "IS_LIBRARY=1\n" )
        self.handle.write( "CLAM_PATH=$(TOP)\n" )

    def addFeature( self, feature ) :
        escapedFeature = re.sub( "-", "_", feature )
        self.handle.write( "ENABLED_%s=@ENABLED_%s@\n"%(escapedFeature.upper(),escapedFeature.upper()) )

    def addLibrary( self, libname ) :
        escapedLibname = re.sub( "-", "_", libname )
        self.handle.write( "USING_%s=@USING_%s@\n"%(escapedLibname.upper(),escapedLibname.upper()) )
        

    def __del__( self ) :
        self.handle.close()

