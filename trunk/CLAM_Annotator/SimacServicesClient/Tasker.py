import ServiceStub
import os, sys
import urlparse, urllib
from os import path

#TODO El projecte incloura la descripcio d'aquest (el que ha generat la tasca)
#  --> al final ho tindra el mateix source de metadata, no problem
#TODO CANVIS QUE CAL FER:
#	~~separar be en metodes tot aixo
if __name__ == "__main__" :
	if len(sys.argv) != 3:
		print "CLIENT - Usage: ./Client <TASKFILE> <PROJECTNAME>"
		sys.exit(0)

	client=Client( sys.argv[1], sys.argv[2] )
	client.processTask()
	#client.runAnnotator()
	os.system("touch I\ Lost\ You.mp3")
	client.uploadChanges()


clamAnnotatorProjectSkeleton = """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>
  <Schema>%s.sc</Schema>
  <Extractor>ClamExtractorExample</Extractor>
  <Songs>
%s  </Songs>
</Project>"""

clamAnnotatorProjectSongSkeleton = """    <Song>
      <SoundFile>%s</SoundFile>
    </Song>
"""

class Client:
	def __init__(self, taskfile, projectname):
		self.retrieveTask( taskfile)
		self.projectname = projectname
		self.songlisting = {}

	def retrieveTask(self, location):
		#TODO in the future it should be an XML file (cool yeah)
		try:
			self.task = open( location, 'r').read()
		except IOError:
			self.quitError("ERROR: reading file <%s>. Maybe it does not exist"%location)

	def processTask(self):
		lines = self.task.splitlines()
		if not self.correctParameters( lines ):
			self.quitError("TaskFile error: malformed file, some field is wrong or missing!")
	
		ids, descriptors, contentlocatoruri, metadataprovideruri = self.extractParameters(lines)
		self.contentlocator = ServiceStub.ContentLocator(contentlocatoruri)
		self.metadataprovider = ServiceStub.MetadataProvider(metadataprovideruri)
	
		projectsonglisting = ""
		print "\nRetrieving data"
	
		for id in ids:
			#Grab audio file
			locations = self.contentlocator.LocateId( id )
			if locations == "NotFound":
				self.quitError("Service error: file with id=%s not found on the server" % id)
			elif locations == "ServerError":
				self.quitError("Service error: the server is not working properly, a ServerError was received")
			
			url = self.downloadSong( locations)
	
			if url == None:
				self.quitError("ERROR: could not download the song from the given source urls")

			#File name
			audiofilename = urllib.unquote( urlparse.urlparse(url)[2].split('/')[-1] )
			projectsonglisting += clamAnnotatorProjectSongSkeleton % audiofilename
			os.rename('temp',audiofilename)
			#Store modification file for each pool to keep track of the ones upgraded
			self.songlisting[audiofilename] = os.path.getmtime(audiofilename)

			pool = self.metadataprovider.QueryDescriptors( id, descriptors )	
			self.createFile( audiofilename, '.pool', pool )
	
		self.createFile(self.projectname, '.pro', clamAnnotatorProjectSkeleton % ( self.projectname, projectsonglisting ) )
	
		schema = self.metadataprovider.QuerySchema( descriptors )
		self.createFile( self.projectname, '.sc', schema )
	
		#Print log
		print "\n\tTASK PROCESSING FINISHED"
		print "-Downloaded audio files"
		print "-Created files %s and %s\n" % ( self.projectname+".sc", self.projectname+".pro" )
		print "LOG:"
		print self.songlisting

	def uploadChanges(self):
		print "LOOKING FOR MODIFIED FILES:"
		for song in self.songlisting.keys():
			if self.songlisting[song] != os.path.getmtime(song):
				print " > File '%s' modified"  % song
			else:
				print " > File '%s' NOT modified"  % song

	def correctParameters(self, lines):
		return ( lines[0].split('=')[0] == 'id' ) and ( lines[1].split('=')[0] == 'descriptors' )\
			and ( lines[2].split('=')[0] == 'contentlocator' ) and ( lines[3].split('=')[0] == 'metadataprovider' )

	def downloadSong( self, locations):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		for url in locations.splitlines():
			try:
				print "Trying "+url+" ..."
				stream = urllib.urlopen(url)
				file = open( "temp", 'w')
				file.write( stream.read() )
				file.close()
				ok = True
				print "\tOK"
				return url
			except:
				print "\tERROR"
				continue
		return None
	
	def extractParameters(self, lines):
		ids = lines[0].split('=')[1].split(',')
		descriptors = lines[1].split('=')[1].split(',')
		contentlocator = lines[2].split('=')[1]
		metadataprovider = lines[3].split('=')[1]
		return ids, descriptors, contentlocator, metadataprovider

	def quitError(self, message):
		print message
		sys.exit(-1)

	def createFile(self, name, extension, content):
		file = open( name+extension, 'w' )
		file.write( content )
		file.close()

	def runAnnotator(self):
		result = os.system("Annotator %s.pro"%self.projectname)
		print "RESULT = %d"%result
