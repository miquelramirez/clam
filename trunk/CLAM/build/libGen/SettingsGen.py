import os, re, shelve

def resetHeaderDB() :
    try:
        os.remove("CLAM_Headers")
    except OSError, e:
        pass

class LibGenerator :

    isSourceRE = re.compile(r"[^\.#]+\.[cC][xp]?[xp]?\Z" )
    isHeaderRE = re.compile( r"[^\.#].+\.[hH][xp]?[xp]?\Z" )

    setlibName = re.compile( r"@libname@" )
    setXMLRE = re.compile( r"@XML@" )
    setALSARE = re.compile( r"@ALSA@" )
    setFFTWRE = re.compile( r"@FFTW@" )
    setFLTKRE = re.compile( r"@FLTK@" )
    setPthreadsRE = re.compile( r"@PTHREADS@" )
    setRtAudioRE = re.compile( r"@RTAUDIO@" )
    setSourcesListRE = re.compile( r"@sourceslist@" )
    setExtraIncsRE = re.compile( r"@extraincludes@" )
    setExtraLibNames = re.compile( r"@extralibs@" )
    setExtraLibPaths = re.compile( r"@extralibpaths@" )
    removeAllCmds = re.compile( r"@.+@" )


    hdrNormRE = re.compile( r"#include \"(?P<hdr>.+)\"" )

    def __init__( self, libName ) :
        self.libName = libName
        self.libfilename = "CLAM"+libName

        tmplFile = file( "settings.tpl.cfg", "r" )
        self.settingsTmpl = tmplFile.read()
        tmplFile.close()

        [self.libTopPath] = re.search( r"TOP = (.+)", self.settingsTmpl ).groups()

        self.variables={
            'ALSA':"0",
            'FFTW':"0",
            'FLTK':"0",
            'RTAUDIO':"0",
            'PTHREADS':"0",
            'XML':"0"
            }

        self.sourceFilenames = []
        self.condSources = dict()
        self.headers = dict()
        self.libGenTop = "../.."
        self.extraIncludes = []
        self.extraLibPaths = []
        self.extraLibs = []
        self.blackBalled = dict()

    def dependsOn( self, CLAMlibname ) :
        self.extraLibs.append( "CLAM%s"%CLAMlibname )
        self.extraLibPaths.append( "../%s"%CLAMlibname )

    def blackBall( self, name ) :
        self.blackBalled[name]=True

    def isBlackBalled( self, name ) :
        try:
            return self.blackBalled[name]
        except KeyError :
            return False

    def contents( self, basePath ) :
        for item in os.listdir( basePath ) :
            fullPath = "%s/%s"%(basePath,item)
            if os.path.isdir( fullPath ) :
                continue
            if self.isBlackBalled( item.split(".")[0]) :
                print "Rejecting %s : it has been black balled"%fullPath
                continue        
            yield item

    def addFile( self, fname, folder, rootFolder="src" ) :
        if rootFolder != "src" :
            self.extraIncludes.append( "$(TOP)/%s"%(rootFolder,) )

        basePath = "%s/%s/%s"%(self.libGenTop, rootFolder, folder)

        if not os.path.exists( basePath ) or not os.path.isdir( basePath ) :
            raise TypeError, "%s is not a CLAM src folder !!"%(basePath,)

        headerDB = shelve.open( "CLAM_Headers", "c" )
        
        def tryHeader( ) :
            filename = "%s.hxx"%fname
            fullPath = "%s/%s"%(basePath, filename )            
            if not os.path.exists( fullPath ) :
                filename = "%s.h"%fname
                fullPath = "%s/%s"%(basePath, filename )            
                if not os.path.exists( fullPath ) :
                    raise TypeError, "%s does not exist!"%fullPath
            self.accountHeader( headerDB, basePath, filename )

        def trySource( ) :            
            filename = "%s.cxx"%fname
            fullPath = "%s/%s"%(basePath, filename )            
            if not os.path.exists( fullPath ) :
                filename = "%s.c"%fname
                fullPath = "%s/%s"%(basePath, filename )            
                if not os.path.exists( fullPath ) :
                    raise TypeError, "%s does not exist!"%fullPath
            self.sourceFilenames.append( "$(TOP)/%s/%s/%s"%(rootFolder, folder, filename ) )


        failed = True
        try:
            trySource()
            self.blackBall( fname )
            failed = False
        except TypeError, e:
            pass

        try:
            tryHeader()
        except TypeError, e:
            if failed : raise e


    def condAddFile( self, package, fname, folder, rootFolder="src" ) :
        if rootFolder != "src" :
            self.extraIncludes.append( "$(TOP)/%s"%(rootFolder,) )

        basePath = "%s/%s/%s"%(self.libGenTop, rootFolder, folder)

        if not os.path.exists( basePath ) or not os.path.isdir( basePath ) :
            raise TypeError, "%s is not a CLAM src folder !!"%(basePath,)

        headerDB = shelve.open( "CLAM_Headers", "c" )
        
        def tryHeader( ) :
            filename = "%s.hxx"%fname
            fullPath = "%s/%s"%(basePath, filename )            
            if not os.path.exists( fullPath ) :
                filename = "%s.h"%fname
                fullPath = "%s/%s"%(basePath, filename )            
                if not os.path.exists( fullPath ) :
                    raise TypeError, "%s does not exist!"%fullPath
            self.accountHeader( headerDB, basePath, filename )

        def trySource( ) :            
            filename = "%s.cxx"%fname
            fullPath = "%s/%s"%(basePath, filename )            
            if not os.path.exists( fullPath ) :
                filename = "%s.c"%fname
                fullPath = "%s/%s"%(basePath, filename )            
                if not os.path.exists( fullPath ) :
                    raise TypeError, "%s does not exist!"%fullPath
            condition = "USING_%s"%package.upper()
            if not self.condSources.has_key(condition) :
                self.condSources[condition] = list()
            self.condSources[condition].append( "$(TOP)/%s/%s/%s"%(rootFolder, folder, filename ) )

        try:
            trySource()
            self.blackBall( fname )
        except TypeError, e:
            pass

        try:
            tryHeader()
        except TypeError, e:
            raise e




    def accountHeader( self, headerDB, basePath, fname ) :
        try:
            moduleName = headerDB[ fname ]
            print "Skipping %s, already in module %s..."%(fname, moduleName)
            
        except KeyError, e :
            try:
                self.headers[basePath].append( fname )
            except KeyError, e :
                self.headers[ basePath ] = list()
                self.headers[ basePath ].append( fname )
                
            headerDB[ fname ] = self.libName

    def condAddFolder( self, package, folder, rootFolder="src" ) :
        if rootFolder != "src" :
            self.extraIncludes.append( "$(TOP)/%s"%(rootFolder,) )

        basePath = "%s/%s/%s"%(self.libGenTop, rootFolder, folder)
        
        if not os.path.exists( basePath ) or not os.path.isdir( basePath ) :
            raise TypeError, "%s is not a CLAM src folder !!"%(basePath,)

        headerDB = shelve.open( "CLAM_Headers", "c" )

        conditionName = "USING_%s"%package.upper()
        if not self.condSources.has_key( conditionName ) :
            self.condSources[conditionName] = list()
        
        for fname in self.contents(basePath) :                    
            if ( self.isSourceRE.search( fname ) != None ) :
                self.condSources[conditionName].append( "$(TOP)/%s/%s/%s"%(rootFolder, folder, fname ) )
                continue
            if (self.isHeaderRE.search( fname ) != None ) :
                self.accountHeader( headerDB, basePath, fname )

        headerDB.sync()
        headerDB.close()



    def addFolder( self, folder, rootFolder="src" ) :
        if rootFolder != "src" :
            self.extraIncludes.append( "$(TOP)/%s"%(rootFolder,) )

        basePath = "%s/%s/%s"%(self.libGenTop, rootFolder, folder)
        
        if not os.path.exists( basePath ) or not os.path.isdir( basePath ) :
            raise TypeError, "%s is not a CLAM src folder !!"%(basePath,)

        headerDB = shelve.open( "CLAM_Headers", "c" )
        
        for fname in self.contents(basePath) :                    
            if ( self.isSourceRE.search( fname ) != None ) :
                self.sourceFilenames.append( "$(TOP)/%s/%s/%s"%(rootFolder, folder, fname ) )
                continue
            if (self.isHeaderRE.search( fname ) != None ) :
                self.accountHeader( headerDB, basePath, fname )

        headerDB.sync()
        headerDB.close()
        
    def activate( self, key ) :
        self.variables[key] = "1"


    def deactivate( self, key ) :
        self.variables[key] = "0"

    def generateFiles( self, path ) :

        if not os.path.exists( path ) :
            raise TypeError, "The path fed into LibGenerator.generateFile() must exist!"
        
        libFilesPath = "%s/%s"%(path,self.libName)
        
        if not os.path.exists( libFilesPath  ) :
            os.makedirs( libFilesPath )

        libSettings = self.settingsTmpl

        libSettings = self.setlibName.sub( self.libfilename, libSettings )

        if len( self.extraLibPaths ) > 0 :
            extraLibPathsStr = " ".join( self.extraLibPaths )
            libSettings = self.setExtraLibPaths.sub( extraLibPathsStr, libSettings )

        if len( self.extraLibs ) > 0 :
            extraLibsStr = " ".join( self.extraLibs )
            libSettings = self.setExtraLibNames.sub( extraLibsStr, libSettings )

        sourcesStr = "\\\n\t".join( self.sourceFilenames )

        condSourcesStr = []

        for condition, sourceList in self.condSources.iteritems():
            sourceListStr = " ".join( sourceList )
            currentCondStr = "$(%s? %s)"%(condition,sourceListStr)
            condSourcesStr.append( currentCondStr )

        condSourcesStr = "\\\n\t".join( condSourcesStr )

        sourcesStr = "\\\n\t".join( [sourcesStr, condSourcesStr] )

        libSettings = self.setSourcesListRE.sub( sourcesStr, libSettings )

        if len( self.extraIncludes ) > 0 :
            incDirsStr = "\\\n\t".join( self.extraIncludes )
            libSettings = self.setExtraIncsRE.sub( incDirsStr, libSettings )

        libSettings = self.removeAllCmds.sub( "", libSettings )

        libSettingsFile = file( "%s/%s"%(libFilesPath,"settings.cfg"), "w" )

        libSettingsFile.write( libSettings )

        libSettingsFile.close()

        # And now the includes
        libIncPath = "%s/include/CLAM/%s"%( libFilesPath, self.libName )
        if os.path.exists(  libIncPath ) :
            for filename in os.listdir( libIncPath ) :
                os.remove( "%s/%s"%(libIncPath,filename) )
            os.removedirs( libIncPath )

        os.makedirs( libIncPath )

        # and now we standardize the include directives

        for basePath in self.headers.keys() :
            for header in self.headers[basePath] :
                pureHeader = self.purifyHeader( basePath, header )
                newHeader = file( libIncPath +"/"+header, "w" )
                newHeader.write( pureHeader )
                newHeader.close()

        # and nevermind the bollo..., hrmm, the Makefile
        os.system( "cp Makefile %s"%libFilesPath )

    def purifyHeader( self, basePath, header ) :
        pureHeader = []

        headerDB = shelve.open( "CLAM_Headers", "r" )

        hdrFilename = "%s/%s"%(basePath,header)
        hdrFile = file( hdrFilename, "r" )

        for line in hdrFile :
            match = self.hdrNormRE.search( line )
            if match != None :
                try:
                    module = headerDB[match.groupdict()['hdr']]
                except KeyError :
                    raise TypeError, "Error processing %s : the header %s is not on the DB"%(hdrFilename,match.groupdict()['hdr'] )
                pureHeader.append( self.hdrNormRE.sub( "#include <CLAM/%s/\g<hdr>>"%module, line ) )
            else :
                pureHeader.append( line )
        
        hdrFile.close()

        headerDB.close()

        return "".join(pureHeader)
        

        


        

        
