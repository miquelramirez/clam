#! /usr/bin/python

from commands import getoutput
def queryDiffs(module) :
	fstTag = "tests-passed-on-linux-rel"
	secTag = "tests-passed-on-linux-candidate-rel"
	cmd = "cvs rdiff -r%s -r%s  %s" % (fstTag, secTag, module)
	filename = "cvsdiff-%s" % module
	print "issuing: ",cmd
	out = getoutput(cmd)
	open(filename, "w").write( out )


NewFile = -1
RemovedFile = -2
ChangedFile = 0

def getFilenameAndTags(diff):
	
	skipcmd = 8
	if diff.find("diff -c /dev/null")==0 :
		skipcmd = 18
	fstTagPos = diff.find(":")
	filename = diff[skipcmd:fstTagPos]
	fstTagEndPos = diff.find(" ", fstTagPos)
	secTagPos = diff.rfind(":")
	if secTagPos == fstTagPos : 
		fstTag = diff[fstTagPos+1:-1]
		return (NewFile, filename, fstTag, "new")
	fstTag = diff[fstTagPos+1:fstTagEndPos]
	secTag = diff[secTagPos+1: -1]
	if secTag == "removed" :
		return (RemovedFile, filename, fstTag, secTag)
	return (ChangedFile, filename, fstTag, secTag)


def cleanLogOutput(log) :
	headEnds = log.find("--\nrevision")
	lastLine = log.rfind("\n====")
	return log[headEnds+3:lastLine]

def getAuthors(log) :
	pos = log.find("author: ")
	authors = []
	while pos != -1 :
		end = log.find( ";", pos+8)
		current = log[pos+8:end]
		authors.append(current)
		pos = log.find("author: ", pos + 8 + len(current))
	return authors
		

from sets import Set
def queryLogs(module) :
	filename = "cvsdiff-%s" % module
	lines = open(filename).readlines()
	diffs =  filter(lambda line:line.find("diff -c")==0, lines)
	"\n".join(diffs)
	
	authors = Set()
	logs = []
	for diff in diffs :
		type, file, fst, sec =  getFilenameAndTags(diff)
		cmd = "cvs rlog -N -r%s -r%s %s" % (fst, sec, file)
		print "issuing:",cmd
		log = getoutput(cmd)
		logs.append( cleanLogOutput(log) )
		fileauthors = Set(getAuthors(log))
		print fileauthors
		authors = authors.union( fileauthors )
	return ",".join( authors ), "".join(logs)
		
def testcvs() :
	diffs = queryDiffs("CLAM")
	authors, logs = queryLogs("CLAM")
	print "authors: %s\n logs:\n%s" % (authors, logs)

testcvs()
