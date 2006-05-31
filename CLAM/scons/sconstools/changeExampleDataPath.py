#! /usr/bin/python

import os.path, glob, re, sys

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files


install_path = '/usr/share/smstools'
source_path = install_path

if len(sys.argv) not in [2,3]:
	print 'usage:'
	print sys.argv[0], 'source_path [install_path]'
	print 'where install_path will be pre-appended to example-data'
if len(sys.argv)>1:
	source_path = sys.argv[1]

if len(sys.argv)>2:
	install_path = sys.argv[2]

print
print 'example-data will be taken from:', source_path
print 'and installed into:', install_path
print

xmls = scanFiles( '*.xml', [ source_path+'/example-data' ] )
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
