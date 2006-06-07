import os, re

hdrNormRE = re.compile( r'(?P<prefix>.*)include\s*["](?P<hdr>.+)["].*', re.IGNORECASE )
	
def update_includes_without_db(source, target=None ) :

	if target is None : 
		target = source
	
	newFile = []
	isDirty = False
	fileHandle = file( source, "r" )
	for line in fileHandle :
		match = hdrNormRE.search( line )
		if match is not None :
			newFile.append( hdrNormRE.sub('\g<prefix>include <CLAM/\g<hdr>>', line ))
			isDirty = True            
		else :
			newFile.append( line )

	fileHandle.close()

	if isDirty == True :
		fileHandle = file( target, "w" )        
		fileHandle.write( "".join( newFile ) )        
		fileHandle.close()
	
	return target

