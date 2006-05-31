#! /usr/bin/python

import os.path, glob, re, sys

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files


install_path = '/usr/share/smstools'
if len(sys.argv)>1:
	install_path = sys.argv[1]
print 'example-data will be installed in:', install_path

xmls = scanFiles( '*.xml', [ install_path+'/example-data' ] )
print 'files to process: ', '\n'.join(xmls)
for file in xmls:
	f = open(file)
	content = f.read()
	f.close()
	content = re.sub(r'>example-data/', r'>%s/example-data/' % install_path, content)	
	f = open(file, 'w')
	f.write( content )
	f.close()
	print file, 'has been changed.'
print 'done!'
