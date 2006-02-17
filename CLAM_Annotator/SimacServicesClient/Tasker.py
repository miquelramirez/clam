import os, sys
import pdb
import gzip
import urlparse, urllib, urllib2
from threading import Thread

import xml.dom.ext
from xml.dom.ext.reader.Sax2 import FromXmlStream
from xml.sax import saxutils

from Pool import Pool
import ServiceStub
import HttpFormPost

clamAnnotatorProjectSkeleton = """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>
  <!-- <Description>%s</Description> -->
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
				self.printfunction(u"\n  WARNING: it was impossible to download the file with id=%s from the given source urls.\n\n" % id)
				continue

			#File name
			audiofilename = urllib.unquote( urlparse.urlparse(url)[2].split('/')[-1] )
			projectsonglisting += clamAnnotatorProjectSongSkeleton % audiofilename
			os.rename( self.path+'temp', self.path+audiofilename )

			#Store modification file for each pool to keep track of the ones upgraded
			self.songlisting[audiofilename] = [ os.path.getmtime( self.path+audiofilename ), False, id ]

			try:
				pool = self.metadataprovider.QueryDescriptors( id, descriptors )
			except:
				raise TaskerError("Access error\nError accessing metadata provider server.")
			
			self.createFile( audiofilename, '.pool', pool )

		if projectsonglisting == "":
			raise TaskerError("Access error\nNo file from the task could be downloaded. Check the task file for correct ids.")

		self.createFile(self.projectname, '.pro', clamAnnotatorProjectSkeleton % ( self.description, self.projectname, projectsonglisting ) )
	
	
		#Print log
		self.printfunction(u"\n  == Task processing finished ==\n")
		
		self.printfunction(u" - Created files %s and %s\n" % ( self.projectname+".pro", self.projectname+".sc" ))
		self.printfunction(u" - Downloaded %d audio file(s) and generated the corresponding pools\n" % len(self.songlisting) )


	def downloadSong(self, locations):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		if len(locations) == 0:
			return None

		for url in locations.splitlines():
			try:
				"""if urlparse.urlparse (url)[1] in ServiceStub.NoProxiesFor:
					proxy_support = urllib2.ProxyHandler( {} )
					opener = urllib2.build_opener(proxy_support)
					urllib2.install_opener(opener)
				else:
					proxy_support = urllib2.ProxyHandler( ServiceStub.Proxies )
					opener = urllib2.build_opener(proxy_support)
					urllib2.install_opener(opener)"""

				self.printfunction(u" - Trying '%s'" % url)
				if urlparse.urlparse(url)[1] in ServiceStub.NoProxiesFor :
					stream = urllib2.urlopen(url)
				else:
					stream = urllib.urlopen(url,None,ServiceStub.Proxies)
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

		de=self.task.getElementsByTagName("Description")[0]
		de.normalize()
		self.description=saxutils.escape(de.firstChild.data)

		for id in self.task.getElementsByTagName("ID"):
			id.normalize()
			ids.append(id.firstChild.data)
		
		for desc in self.task.getElementsByTagName("Descriptor"):
			desc.normalize()
			if desc.hasAttribute('modify') and desc.getAttribute('modify')=='yes':
				scopename=desc.firstChild.data.split('::')
				self.modifydescriptors.append( (scopename[0],scopename[1]) )
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
			songpool=self.path+song+".pool"
			if self.songlisting[song][0] != os.path.getmtime(songpool):
				self.printfunction(u" - File '%s' modified\n"  % songpool)
				self.songlisting[song][1] = True
				modified.append(song)
			else:
				self.printfunction(u" - File '%s' NOT modified\n"  % songpool)
		return modified

	def uploadChanges(self):
		uploadfile='uploadfile.xml.gz'
		self.printfunction(u"\n  == Uploading ==\n")

		self.printfunction(u" - Generating file to upload\n")
		uf = gzip.GzipFile( uploadfile, 'w')

		uf.write("<?xml version='1.0' encoding='UTF-8'?>\n")
		uf.write("<UploadPools>\n")
		for song in self.songlisting.keys():
			#if modified
			if self.songlisting[song][1]:  
				uf.write("<Song id='%s'>" % self.songlisting[song][2] )
				poolfile=self.path+song+".pool"
				pool=Pool(poolfile)
				temppool=Pool()

				#Extract desired attributes
				for (scope,name) in self.modifydescriptors:
					temppool.InsertAttribute(pool,scope,name,scope,name)
				xml.dom.ext.PrettyPrint(temppool.doc.documentElement, uf)
				uf.write("</Song>\n")

		uf.write("</UploadPools>\n")
		uf.close()

		self.printfunction(u" - Uploading the modified descriptor file to the server\n")
		
		result=self.metadataprovider.UploadPackedDescriptors(uploadfile)
		if result != "OK":
			self.printfunction(u"\n - ERROR in the server when processing the file: %s\n" % result )
		else:
			self.printfunction(u" - File successfully uploaded: received %s\n"%result)

		#Clean!
		os.remove('uploadfile.xml.gz')


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
			result = os.system( "Annotator %s.pro &> /dev/null" % (self.path+self.projectname) )
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
		tasker.setParameters( sys.argv[1], sys.argv[2], '/home/xoliver/temp' )
		tasker.processTask()
		tasker.runAnnotator()
		import time
		time.sleep(1)
		os.system("touch /home/xoliver/temp/I\ Lost\ You.mp3.pool")
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
