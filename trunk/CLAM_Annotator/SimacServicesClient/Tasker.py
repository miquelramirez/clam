import ServiceStub
import os, sys
import urlparse, urllib
from threading import Thread

from xml.dom.ext.reader.Sax2 import FromXmlStream

#TODO CANVIS QUE CAL FER:
#	separar be en metodes tot aixo (bastant fet)

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

class ProcessThread(Thread):
	def __init__(self):
		Thread.__init__(self)

	def run(self):
		pass

class TaskerError(Exception):
	def __init__(self, message):
		self.message=message

	def __str__(self):
		return self.message

class Tasker:
	def __init__(self, printfunction=sys.stdout.write):
		self.songlisting = {}
		self.printfunction = printfunction
		self.path = os.getcwd() + os.sep
		self.modifydescriptors = []

	def setParameters(self, taskfile, projectname, path=os.getcwd() ):
		self.retrieveTask( taskfile)
		self.projectname = projectname
		self.path = path + os.sep

	def retrieveTask(self, location):
		try:
			self.task = FromXmlStream(location)
		except:
			raise TaskerError("Task file error\nReading file '%s'. It doesn't exist or it is a malformed XML file." % location)

	def processTask(self):
		ids, descriptors, contentlocatoruri, metadataprovideruri = self.extractParameters()

		self.contentlocator = ServiceStub.ContentLocator(contentlocatoruri)
		self.metadataprovider = ServiceStub.MetadataProvider(metadataprovideruri)
	
		projectsonglisting = ""
		self.printfunction(u"\n  == Retrieving data ==\n")
		
		schema = self.metadataprovider.QuerySchema( descriptors )
		self.createFile( self.projectname, '.sc', schema )
	
		for id in ids:
			#Grab audio file
			try:
				locations = self.contentlocator.LocateId( id )
			except:
				raise TaskerError("Access error\nError accessing content locator server.")

			if locations == "ServerError": #Stop execution
				raise TaskerError("Service error\nThe server is not working properly, a ServerError was received.")
			elif locations == "NotFound": #Ignore song
				self.printfunction(u"\n  WARNING: File with id=%s not found on the server. Id will be ignored.\n" % id)
				continue
			
			url = self.downloadSong( locations)
	
			if url == None:
				self.printfunction(u"\n  WARNING: it was impossible to download the file with id=%s from the given source urls.\n" % id)
				continue

			#File name
			audiofilename = urllib.unquote( urlparse.urlparse(url)[2].split('/')[-1] )
			projectsonglisting += clamAnnotatorProjectSongSkeleton % audiofilename
			os.rename( self.path+'temp', self.path+audiofilename )

			#Store modification file for each pool to keep track of the ones upgraded
			self.songlisting[audiofilename] = [ os.path.getmtime(audiofilename), False ]

			try:
				pool = self.metadataprovider.QueryDescriptors( id, descriptors )
			except:
				raise TaskerError("Access error\nError accessing metadata provider server.")
			
			self.createFile( audiofilename, '.pool', pool )

		if projectsonglisting == "":
			raise TaskerError("Access error\nNo file from the task could be downloaded. Check the task file for correct ids.")

		self.createFile(self.projectname, '.pro', clamAnnotatorProjectSkeleton % ( self.projectname, projectsonglisting ) )
	
	
		#Print log
		self.printfunction(u"\n  == Task processing finished ==\n")
		
		self.printfunction(u" - Created files %s and %s\n" % ( self.projectname+".pro", self.projectname+".sc" ))
		self.printfunction(u" - Downloaded %d audio file(s) and generated the corresponding pools\n" % len(self.songlisting) )

	#TODO thread en algun lloc de per aqui

	def downloadSong(self, locations):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		if len(locations) == 0:
			return None

		for url in locations.splitlines():
			try:
				self.printfunction(u" - Trying '%s'" % url)
				stream = urllib.urlopen(url)
				file = open( self.path+"temp", 'w')
				file.write( stream.read() )
				file.close()
				ok = True
				self.printfunction(u"     (OK)\n")
				return url
			except:
				self.printfunction(u"     (ERROR)\n")
				continue
				
		return None
	
	def extractParameters(self):
		ids = []
		descriptors = []

		for id in self.task.getElementsByTagName("ID"):
			id.normalize()
			ids.append(id.firstChild.data)
		
		for desc in self.task.getElementsByTagName("Descriptor"):
			desc.normalize()
			if desc.hasAttribute('modify') and desc.getAttribute('modify'):
				self.modifydescriptors.append(desc.firstChild.data)
			descriptors.append(desc.firstChild.data)

		contentlocator = self.task.getElementsByTagName("ContentLocator")
		metadataprovider = self.task.getElementsByTagName("MetadataProvider")

		if ( len(ids)<1 ) or ( len(descriptors)<1 ) \
			or len(contentlocator)!=1 or len(metadataprovider)!=1 :
			raise TaskerError("Task file error\nMalformed file, some field is wrong or missing!")
		else:
			metadataprovider = metadataprovider[0].firstChild.data
			contentlocator = contentlocator[0].firstChild.data

		return ids, descriptors, contentlocator.strip(), metadataprovider.strip()

	def getModified(self):
		modified = []
		self.printfunction(u"\n  == Looking for modified files ==\n")
		for song in self.songlisting.keys():
			songpool=song+".pool"
			if self.songlisting[song][0] != os.path.getmtime(songpool):
				self.printfunction(u" - File '%s' modified\n"  % songpool)
				self.songlisting[song][1] = True
				modified.append(song)
			else:
				self.printfunction(u" - File '%s' NOT modified\n"  % songpool)
		return modified

	def uploadChanges(self):
		self.printfunction(u"\n  == Uploading ==\n")
		self.printfunction(u" - Fake!!\n")

	def createFile(self, name, extension, content):
		try:
			file = open( self.path+name+extension, 'w' )
			file.write( content )
			file.close()
		except:
			raise TaskerError("File write error\nError writing the files, maybe the directory does not exist!")

	def runAnnotator(self):
		self.printfunction(u"\n  == CLAM-Annotator ==\n")
		self.printfunction(u" - Launching...\n")
		if sys.platform != 'win32':
			result = os.system( "Annotator %s.pro &> /dev/null" % self.projectname )
		else:
			raise TaskError("Run Annotator error\nWindows execution of the Annotator is still not managed.")
		self.printfunction( "RESULT = %d" % result )
		self.printfunction(u" - Finalized\n")


if __name__ == "__main__" :
	if len(sys.argv) != 3:
		print("CLIENT - Usage: ./Tasker <TASKFILE> <PROJECTNAME>")
		sys.exit(0)
	try:
		tasker=Tasker()
		tasker.setParameters( sys.argv[1], sys.argv[2] )
		tasker.processTask()
		#tasker.runAnnotator()
		#os.system("touch I\ Lost\ You.mp3.pool")
		modified = tasker.getModified()
		if len(modified)==0:
			print "\n - No descriptor pool modified. Exitting without uploading anything.\n"
		else:
			print "\n - The following descriptor pools will be uploaded:\n  -" + ('\n  - ').join(modified)
			answer = raw_input("\n > Do you want to do it? (y/n)  ") 
			if answer.strip() == 'y':
				tasker.uploadChanges()
			else:
				print "\n - No descriptor uploaded\n"
	except TaskerError, err:
		print err
