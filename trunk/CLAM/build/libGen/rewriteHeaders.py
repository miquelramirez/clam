#!/usr/bin/python

import os, re, shelve, getopt, sys

hdrNormRE = re.compile( r"#include \"(?P<hdr>.+)\"" )
isSourceRE = re.compile(r"[^\.#]+\.[cC][xp]?[xp]?\Z" )
isHeaderRE = re.compile( r"[^\.#].+\.[hH][xp]?[xp]?\Z" )

def sources( folder ) :
    for item in os.listdir( folder ) :
        if (isSourceRE.search( item ) != None or
            isHeaderRE.search( item ) != None ) :
            yield "%s/%s"%(folder,item)
        else :
            fullPath = "%s/%s"%(folder,item)
            if os.path.isdir( fullPath ) :
                yield fullPath

def processFolder( headerDB, folder, mustRecurse ) :
    for fname in sources( folder ) :
        if mustRecurse and os.path.isdir( fname ) :
            processFolder( headerDB, fname, True )
        else :
            processSingleFile( headerDB, fname )

def processSingleFile( headerDB, fname ) :
    print fname
    newFile = []
    isDirty = False
    fileHandle = file( fname, "r" )
    for line in fileHandle :
        match = hdrNormRE.search( line )
        if match != None :
            try:
                module = headerDB[match.groupdict()['hdr']]
                newFile.append( hdrNormRE.sub( "#include <CLAM/%s/\g<hdr>>"%module, line ) )
                isDirty = True            
            except KeyError :
                newFile.append(line)
        else :
            newFile.append( line )

    fileHandle.close()

    if isDirty == True :
        fileHandle = file( fname, "w" )        
        fileHandle.write( "".join( newFile ) )        
        fileHandle.close()

def usage() :
    print """
This script processes inplace the input file/folder looking for CLAM includes and converts
them to the 'library form'. For instance, if a source file happens to contain

    #include "Processing.hxx"

it will be converted to :

    #include <CLAM/Core/Processing.hxx>

The script accepts the following parameters:
    -h, --help              Shows this help
    -i, --input   <path>    File or folder to convert
    -r, --recurse           Recurses into subfolders ( if the input parameter was a folder )

    """

def main() :

    try:
        opts, args = getopt.getopt( sys.argv[1:], "hi:r", ["help","input=","recurse"] )
    except getopt.GetoptError :
        usage()
        sys.exit(2)
    
    inputFname = ""
    mustRecurse = False

    for option,argument in opts :
        if option in ( "-h", "--help" ) :
            usage()
            sys.exit(1)
        if option in ( "-i", "--input" ) :
            inputFname = argument
        if option in ( "-r", "--recurse" ) :
            mustRecurse = True

    if len(inputFname) == 0 :
        print "No input was specified!!!"
        usage()
        sys.exit(2)

    if not os.path.exists( inputFname ) :
        print "Specified input %s is not a file or folder!!!"
        usage()
        sys.exit(2)

    headerDB = shelve.open( "CLAM_Headers", "r" )

    if len( headerDB.keys() ) == 0 :
        print "There is some problem with the CLAM Headers Database, either it has"
        print "not been generated or it has got somehow corrupted. Rerun generateLibs.py"
        sys.exit(2)

    if os.path.isfile( inputFname ) :
        processSingleFile( headerDB, inputFname )
    else :
        processFolder( headerDB, inputFname, mustRecurse )

    headerDB.close()

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script!"
    sys.exit(2)
