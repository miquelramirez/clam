import os

def store_headers_in_db(  db ) :
	for _,_,headerlist in os.walk( 'include/CLAM' ) :
		for header in headerlist :
			db[header] = True
