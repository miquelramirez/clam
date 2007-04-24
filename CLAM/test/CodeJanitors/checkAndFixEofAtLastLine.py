import sys, os, glob


def endsWithEndOfLine(filename) :
	try:
		return len(file(filename).readlines().pop().strip())==0
	except :
		print "Empty ", filename

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files

def recursiveDirs(root) :
	return [root] + filter( (lambda a : a.rfind( ".svn")==-1 ),  [ a[0] for a in os.walk(root)]  )


if __name__=="__main__" :
	print recursiveDirs(".")
	for f in scanFiles("*xx", recursiveDirs('.')) : 
		if endsWithEndOfLine(f) : continue
		print f
		content = file(f).read()
		fd=open(f,"w")
		fd.write(content)
		fd.write("\n")
		fd.close()
		


