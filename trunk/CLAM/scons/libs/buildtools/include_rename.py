import os, re

hdrNormRE = re.compile( r'.*#include.*[<"](?P<hdr>.+)[>"].*', re.IGNORECASE )

def update_includes( header_db, source, target=None ) :

	if target is None : 
		target = source
	
	newFile = []
	isDirty = False
	fileHandle = file( source, "r" )
	for line in fileHandle :
		match = hdrNormRE.search( line )
		if match is not None :
			try:
				header_db[match.groupdict()['hdr']]
				newFile.append( hdrNormRE.sub( '#include <CLAM/\g<hdr>>', line ) )
				isDirty = True            
			except KeyError :
				newFile.append(line)
		else :
			newFile.append( line )

	fileHandle.close()

	if isDirty == True :
		fileHandle = file( target, "w" )        
		fileHandle.write( "".join( newFile ) )        
		fileHandle.close()
	
	return target
	

