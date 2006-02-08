import ServiceStub
import os, sys
import urlparse, urllib
from os import path

from xml.dom.ext.reader.Sax2 import FromXmlStream

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
		try:
			self.task = FromXmlStream(location)
		except:
			raise ClientError("Task file error\nReading file '%s'. It doesn't exist or it is a malformed XML file." % location)

	def processTask(self):
		ids, descriptors, contentlocatoruri, metadataprovideruri = self.extractParameters()
		self.contentlocator = ServiceStub.ContentLocator(contentlocatoruri)
		self.metadataprovider = ServiceStub.MetadataProvider(metadataprovideruri)
	
		projectsonglisting = ""
		self.printfunction(u"\n  == Retrieving data ==\n")
	
		for id in ids:
			#Grab audio file
			try:
				locations = self.contentlocator.LocateId( id )
			except:
				raise ClientError("Access error\nError accessing content locator server.")

			if locations == "NotFound":
				raise ClientError("Service error\nFile with id=%s not found on the server." % id)
			elif locations == "ServerError":
				raise ClientError("Service error\nThe server is not working properly, a ServerError was received.")
			
			url = self.downloadSong( locations)
	
			if url == None:
				raise ClientError("Download error\nCould not download the song from the given source urls.")

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
		self.printfunction(u"\n  == Task processing finished ==\n")
		self.printfunction(u" - Downloaded audio files\n")
		self.printfunction(u" - Created files %s and %s\n" % ( self.projectname+".sc", self.projectname+".pro" ))

	def downloadSong( self, locations):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		for url in locations.splitlines():
			try:
				self.printfunction(u" - Trying '%s'" % url)
				stream = urllib.urlopen(url)
				file = open( "temp", 'w')
				file.write( stream.read() )
				file.close()
				ok = True
				self.printfunction(u"     (OK)\n")
				return url
			except:
				self.printfunction("     (ERROR)\n")
				continue
				
		return None
	
	def extractParameters(self):
		dataerror = ClientError("Task file error\nMalformed file, some field is wrong or missing!")
		ids = []
		descriptors = []

		for id in self.task.getElementsByTagName("ID"):
			id.normalize()
			ids.append(id.firstChild.data)
		
		for desc in self.task.getElementsByTagName("Descriptor"):
			desc.normalize()
			descriptors.append(desc.firstChild.data)

		contentlocator = self.task.getElementsByTagName("ContentLocator")[0].firstChild.data
		metadataprovider = self.task.getElementsByTagName("MetadataProvider")[0].firstChild.data

		return ids, descriptors, contentlocator, metadataprovider

	def uploadChanges(self):
		self.printfunction(u"\n  == Looking for modified files ==\n")
		for song in self.songlisting.keys():
			songpool=song+".pool"
			if self.songlisting[song] != os.path.getmtime(songpool):
				self.printfunction(u" - File '%s' modified\n"  % songpool)
			else:
				self.printfunction(u" - File '%s' NOT modified\n"  % songpool)


	def createFile(self, name, extension, content):
		file = open( name+extension, 'w' )
		file.write( content )
		file.close()

	def runAnnotator(self):
		result = os.system( "Annotator %s.pro" % self.projectname )
		self.printfunction( "RESULT = %d" % result )


if __name__ == "__main__" :
	if len(sys.argv) != 3:
		self.printfunction("CLIENT - Usage: ./Client <TASKFILE> <PROJECTNAME>")
		sys.exit(0)

	client=Client()
	client.setParameters( sys.argv[1], sys.argv[2] )
	client.processTask()
	#client.runAnnotator()
	os.system("touch I\ Lost\ You.mp3")
	client.uploadChanges()
