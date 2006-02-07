import ServiceStub
import os, sys
import urlparse, urllib
from os import path

#TODO El projecte incloura la descripcio d'aquest (el que ha generat la tasca)
#  --> al final ho tindra el mateix source de metadata, no problem
#TODO CANVIS QUE CAL FER:
#	~~separar be en metodes tot aixo


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

class ClientError(Exception):
	def __init__(self, message):
		self.message=message

	def __str__(self):
		return self.message

class Client:
	def __init__(self, printfunction=sys.stdout.write):
		self.songlisting = {}
		self.printfunction=printfunction

	def setParameters(self, taskfile, projectname ):
		self.retrieveTask( taskfile)
		self.projectname = projectname

	def retrieveTask(self, location):
		#TODO in the future it should be an XML file (cool yeah)
		try:
			self.task = open( location, 'r').read()
		except IOError:
			raise "Task file error\nReading file <%s>. Maybe it does not exist."%location

	def processTask(self):
		lines = self.task.splitlines()
		if not self.correctParameters( lines ):
			raise "Task file error\nMalformed file, some field is wrong or missing!"
	
		ids, descriptors, contentlocatoruri, metadataprovideruri = self.extractParameters(lines)
		self.contentlocator = ServiceStub.ContentLocator(contentlocatoruri)
		self.metadataprovider = ServiceStub.MetadataProvider(metadataprovideruri)
	
		projectsonglisting = ""
		self.printfunction("\nRetrieving data")
	
		for id in ids:
			#Grab audio file
			try:
				locations = self.contentlocator.LocateId( id )
			except:
				raise ClientError("Access error\nError accessing content locator server.")

			if locations == "NotFound":
				raise "Service error\nFile with id=%s not found on the server." % id
			elif locations == "ServerError":
				raise "Service error\nThe server is not working properly, a ServerError was received."
			
			url = self.downloadSong( locations)
	
			if url == None:
				raise "Download error\nCould not download the song from the given source urls."

			#File name
			audiofilename = urllib.unquote( urlparse.urlparse(url)[2].split('/')[-1] )
			projectsonglisting += clamAnnotatorProjectSongSkeleton % audiofilename
			os.rename('temp',audiofilename)
			#Store modification file for each pool to keep track of the ones upgraded
			self.songlisting[audiofilename] = os.path.getmtime(audiofilename)

			try:
				pool = self.metadataprovider.QueryDescriptors( id, descriptors )
			except:
				raise ClientError("Access error\nError accessing metadata provider server.")
			
			self.createFile( audiofilename, '.pool', pool )
	
		self.createFile(self.projectname, '.pro', clamAnnotatorProjectSkeleton % ( self.projectname, projectsonglisting ) )
	
		schema = self.metadataprovider.QuerySchema( descriptors )
		self.createFile( self.projectname, '.sc', schema )
	
		#Print log
		self.printfunction("\n\tTASK PROCESSING FINISHED")
		self.printfunction("-Downloaded audio files")
		self.printfunction("-Created files %s and %s\n" % ( self.projectname+".sc", self.projectname+".pro" ))

	def uploadChanges(self):
		self.printfunction("LOOKING FOR MODIFIED FILES:")
		for song in self.songlisting.keys():
			if self.songlisting[song] != os.path.getmtime(song):
				self.printfunction(" > File '%s' modified"  % song)
			else:
				self.printfunction(" > File '%s' NOT modified"  % song)

	def correctParameters(self, lines):
		return ( lines[0].split('=')[0] == 'id' ) and ( lines[1].split('=')[0] == 'descriptors' )\
			and ( lines[2].split('=')[0] == 'contentlocator' ) and ( lines[3].split('=')[0] == 'metadataprovider' )

	def downloadSong( self, locations):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		for url in locations.splitlines():
			try:
				self.printfunction("Trying "+url)
				stream = urllib.urlopen(url)
				file = open( "temp", 'w')
				file.write( stream.read() )
				file.close()
				ok = True
				self.printfunction("..OK")
				return url
			except:
				self.printfunction("..ERROR")
				continue
				
		return None
	
	def extractParameters(self, lines):
		ids = lines[0].split('=')[1].split(',')
		descriptors = lines[1].split('=')[1].split(',')
		contentlocator = lines[2].split('=')[1]
		metadataprovider = lines[3].split('=')[1]
		return ids, descriptors, contentlocator, metadataprovider

	def createFile(self, name, extension, content):
		file = open( name+extension, 'w' )
		file.write( content )
		file.close()

	def runAnnotator(self):
		result = os.system("Annotator %s.pro"%self.projectname)
		self.printfunction("RESULT = %d"%result)


if __name__ == "__main__" :
	if len(sys.argv) != 3:
		self.printfunction("CLIENT - Usage: ./Client <TASKFILE> <PROJECTNAME>")
		sys.exit(0)

	client=Client( sys.argv[1], sys.argv[2] )
	client.processTask()
	#client.runAnnotator()
	os.system("touch I\ Lost\ You.mp3")
	client.uploadChanges()

