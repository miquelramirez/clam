import os, sys
import gzip
import urlparse, urllib, urllib2

import xml.dom.ext
from xml.dom.ext.reader.Sax2 import FromXmlStream
from xml.sax import saxutils

from Pool import Pool
import ServiceStub

import shelve

clamAnnotatorProjectSkeleton = """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>
  <Description>%s</Description>
  <Schema>%s.sc</Schema>
  <Extractor>ClamExtractorExample</Extractor>
  <Songs>
%s  </Songs>
</Project>"""

clamAnnotatorProjectSongSkeleton = """    <Song>
      <SoundFile>%s</SoundFile>
    </Song>
"""

class TaskerError( Exception ):
	def __init__( self, message ):
		self.message=message

	def __str__( self ):
		return self.message

class Tasker:
	def __init__( self, printfunction=sys.stdout.write ):
		self.config = shelve.open( 'config.dict',writeback=True )
		self.printfunction = printfunction

	def setParameters( self, taskfile, projectname, path=os.getcwd() ):
		self.config['taskfile'] = taskfile
		self.config['projectname'] = projectname
		self.config['path'] = path + os.sep
		self.config['songlisting'] = {}
		self.config['modifydescriptors'] = []

	def _retrieveTask( self, location ):
		try:
			return FromXmlStream( location )
		except:
			raise TaskerError( "Task file error\nReading file '%s'. It doesn't exist or it is a malformed XML file." % location )

	def processTask( self ):
		try:
			task = self._retrieveTask( self.config['taskfile'] )
			projectname = self.config['projectname']
			path = self.config['path']
		except KeyError:
			raise TaskerError( "Workflow error\nCould not restore configuration, make sure you follow the usage process." )

		ids, descriptors, contentlocatoruri, metadataprovideruri, description = self._extractParameters( task )
		self.config['metadataprovideruri']=metadataprovideruri

		contentlocator = ServiceStub.ContentLocator( contentlocatoruri )
		metadataprovider = ServiceStub.MetadataProvider( metadataprovideruri )
	
		projectsonglisting = ""
		self.printfunction( u"\n  == Retrieving data ==\n" )
		
		try:
			schema = metadataprovider.QuerySchema( descriptors )
		except:
			raise TaskerError( "Access error\nError accessing metadata provider server." )

		self._createFile( path, projectname, '.sc', schema )
	
		for id in ids:
			#Grab audio file
			try:
				locations = contentlocator.LocateId( id )
			except:
				raise TaskerError( "Access error\nError accessing content locator server." )

			if locations == "ServerError": #Stop execution
				raise TaskerError( "Service error\nThe server is not working properly, a ServerError was received." )
			elif locations == "NotFound": #Ignore song
				self.printfunction( u"\n  WARNING: File with id=%s not found on the server. Id will be ignored.\n" % id )
				continue
			
			audiofilename = self._downloadSong( locations, path )
	
			if audiofilename == None:
				self.printfunction( u"\n  WARNING: it was impossible to download the file with id=%s from the given source urls.\n\n" % id )
				continue

			#Project
			projectsonglisting += clamAnnotatorProjectSongSkeleton % audiofilename

			try:
				pool = metadataprovider.QueryDescriptors( id, descriptors )
			except:
				raise TaskerError( "Access error\nError accessing metadata provider server." )
			
			self._createFile( path, audiofilename, '.pool', pool )
			#Store modification file for each pool to keep track of the ones upgraded
			self.config['songlisting'][audiofilename] = id

		if projectsonglisting == "":
			raise TaskerError( "Access error\nNo file from the task could be downloaded. Check the task file for correct ids." )

		self._createFile( path, projectname, '.pro', clamAnnotatorProjectSkeleton % ( description, projectname, projectsonglisting ) )
	
	
		#Print log
		self.printfunction( u"\n  == Task processing finished ==\n" )
		
		self.printfunction( u" - Created files %s and %s\n" % ( projectname+".pro", projectname+".sc" ) )
		self.printfunction( u" - Downloaded %d audio file(s) and generated the corresponding pools\n" % len( self.config['songlisting'] ) )

		os.utime( self.config['taskfile'], None )


	def _downloadSong( self, locations, savetopath ):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		if len( locations ) == 0:
			return None

		for url in locations.splitlines():
			try:
				"""if urlparse.urlparse ( url )[1] in ServiceStub.NoProxiesFor:
					proxy_support = urllib2.ProxyHandler( {} )
					opener = urllib2.build_opener( proxy_support )
					urllib2.install_opener( opener )
				else:
					proxy_support = urllib2.ProxyHandler( ServiceStub.Proxies )
					opener = urllib2.build_opener( proxy_support )
					urllib2.install_opener( opener )"""

				self.printfunction( u" - Trying '%s'" % url )
				if urlparse.urlparse( url )[1] in ServiceStub.NoProxiesFor :
					stream = urllib2.urlopen( url )
				else:
					stream = urllib.urlopen( url,None,ServiceStub.Proxies )

				if stream.info().type not in [ "audio/mpeg", "application/ogg", "audio/x-wav" ]:
					self.printfunction( u"     ( ERROR )\n" )
					continue
			
				audiofilename = urllib.unquote( urlparse.urlparse( url )[2].split( '/' )[-1] )
				if os.path.exists( savetopath+audiofilename ):
					self.printfunction( u"     Already Downloaded ( OK )\n" )
					return audiofilename
				
				file = open( savetopath+audiofilename, 'w' )
				file.write( stream.read() )
				file.close()
				self.printfunction( u"     ( OK )\n" )
				return audiofilename
			except:
				self.printfunction( u"     ( ERROR )\n" )
				continue
				
		return None

	
	def _extractParameters( self, task ):
		ids = []
		descriptors = []

		for id in task.getElementsByTagName( "ID" ):
			id.normalize()
			ids.append( id.firstChild.data )
		
		for desc in task.getElementsByTagName( "Descriptor" ):
			desc.normalize()
			if desc.hasAttribute( 'modify' ) and desc.getAttribute( 'modify' )=='yes':
				scopename=desc.firstChild.data.split( '::' )
				self.config['modifydescriptors'].append( ( scopename[0],scopename[1] ) )
			descriptors.append( desc.firstChild.data )

		contentlocatoruri = task.getElementsByTagName( "ContentLocator" )
		metadataprovideruri = task.getElementsByTagName( "MetadataProvider" )
		de=task.getElementsByTagName( "Description" )

		if ( len( ids )<1 ) or ( len( descriptors )<1 ) \
			or len( contentlocatoruri )!=1 or len( metadataprovideruri )!=1 \
			or len( de )!=1 :
			raise TaskerError( "Task file error\nMalformed file, some field is wrong or missing!" )
		else:
			metadataprovideruri = metadataprovideruri[0].firstChild.data
			contentlocatoruri = contentlocatoruri[0].firstChild.data
			de[0].normalize()
			description=saxutils.escape( de[0].firstChild.data )

		return ids, descriptors, contentlocatoruri.strip(), metadataprovideruri.strip(), description

	"""
	def getModified( self ):
		try:
			path = self.config['path']
		except KeyError:
			raise TaskerError( "Workflow error\nCould not restore configuration, make sure you follow the usage process." )

		modified = []
		self.printfunction( u"\n  == Looking for modified files ==\n" )
		for song in self.config['songlisting'].keys():
			songpool=path+song+".pool"
			if self.config['songlisting'][song][0] != os.path.getmtime( songpool ):
				self.printfunction( u" - File '%s' modified\n"  % songpool )
				self.config['songlisting'][song][1] = True
				modified.append( song )
			else:
				self.printfunction( u" - File '%s' NOT modified\n"  % songpool )
		return modified
	"""

	def uploadChanges( self, modifiedlist ):
		try:
			path = self.config['path']
			metadataprovider = ServiceStub.MetadataProvider( self.config['metadataprovideruri'] )
			songlisting = self.config['songlisting']
			modifydescriptors = self.config['modifydescriptors']
		except KeyError:
			raise TaskerError( "Workflow error\nCould not restore configuration, make sure you follow the usage process." )

		try:
			uploadfile='uploadfile.xml.gz'
			self.printfunction( u"\n  == Uploading ==\n" )

			self.printfunction( u" - Generating file to upload\n" )
			uf = gzip.GzipFile( uploadfile, 'w' )

			uf.write( "<?xml version='1.0' encoding='UTF-8'?>\n" )
			uf.write( "<UploadPools>\n" )
			for song in songlisting.keys():
				#if it has been modified
				if song in modifiedlist:
					uf.write( "<Song id='%s'>" % songlisting[song] )
					poolfile=path+song+".pool"
					pool=Pool( poolfile )
					temppool=Pool()

					#Extract desired attributes
					for ( scope,name ) in modifydescriptors:
						temppool.InsertAttribute( pool,scope,name,scope,name )
					xml.dom.ext.PrettyPrint( temppool.doc.documentElement, uf )
					uf.write( "</Song>\n" )

			uf.write( "</UploadPools>\n" )
			uf.close()

			self.printfunction( u" - Uploading the modified descriptor file to the server\n" )
			
			result=metadataprovider.UploadPackedDescriptors( uploadfile )
			if result != "OK":
				self.printfunction( u"\n - ERROR in the server when processing the file: %s\n" % result )
			else:
				self.printfunction( u" - File successfully uploaded: received %s\n"%result )

			#Clean!
			os.remove( 'uploadfile.xml.gz' )

			return 0
		except:
			return -1


	def _createFile( self, path, name, extension, content ):
		try:
			file = open( path+name+extension, 'w' )
			file.write( content )
			file.close()
		except:
			raise TaskerError( "File write error\nError writing file '%s', maybe the directory does not exist!" % ( name+extension ) )


	def runAnnotator( self ):
		try:
			path = self.config['path']
			projectname = self.config['projectname']
		except KeyError:
			raise TaskerError( "Workflow error\nCould not restore configuration, make sure you follow the usage process." )

		self.printfunction( u"\n  == CLAM-Annotator ==\n" )
		self.printfunction( u" - Launching...\n" )
		if sys.platform != 'win32':
			result = os.system( "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/lib Annotator %s.pro &> /dev/null" % ( path+projectname ) )
		else:
			raise TaskError( "Run Annotator error\nWindows execution of the Annotator is still not managed." )
		self.printfunction( "RESULT = %d" % result )
		self.printfunction( u" - Finalized\n" )


def usage():
	print( """
	SimacServices Annotator Client

 Usage:

  -To do all the process in a single step
    python Tasker.py do <taskfile> <projectname> [ <path> ]
   
  -To do every step individually (in this order)
    python Tasker.py setparameters <taskfile> <projectname> <path>
    python Tasker.py processtask
    python Tasker.py runannotator
    python Tasker.py upload <audiofiles>
""" )
	sys.exit( 0 )

def TaskerDo( argv ):
	try:
		if len( argv ) != 4 and len( argv ) != 5:
			usage()

		if len( argv ) >= 4:
			taskfile = argv[2]
			projectname = argv[3]
		if len (argv ) == 5:
			path =  argv[4] + os.sep
		else:
			path = os.getcwd() + os.sep

		tasker=Tasker()
		tasker.setParameters( taskfile, projectname, path )
		tasker.processTask()
		tasker.runAnnotator()
		
		tasktime = os.path.getmtime( taskfile )
		modifiedlist = []
		for song in tasker.config['songlisting'].keys():
			songpool = path + os.sep + song + ".pool"
			if os.path.getmtime( songpool ) > tasktime:
				modifiedlist.append( song )

		if len( modifiedlist )==0:
			print "\n - No descriptor pool modified. Exiting without uploading anything.\n"
		else:
			print "\n - The following descriptor pools will be uploaded:\n  -" + ( '\n  - ' ).join( modifiedlist )
			answer = raw_input( "\n > Do you want to do it? (y/n)  " ) 
			if answer.strip() == 'y':
				if tasker.uploadChanges( modifiedlist ) == -1:
					print "\n - Error uploading descriptors\n"
				else:
					print "\n - Descriptors uploaded OK\n"
			else:
				print "\n - No descriptor uploaded\n"
		os.remove( 'config.dict' )
	except TaskerError, err:
		print err

def TaskerSetparameters( argv ):
	if len( argv ) != 5:
		usage()
	
	tasker=Tasker()
	tasker.setParameters( argv[2], argv[3], argv[4] )

def TaskerProcesstask( argv ):
	if len( argv ) != 2:
		usage()
	
	tasker=Tasker()
	tasker.processTask()

def TaskerRunannotator( argv ):
	if len( argv ) != 2:
		usage()
	
	tasker=Tasker()
	tasker.runAnnotator()

def TaskerUpload( argv ):
	poollist = argv[2:]
	if len( poollist ) == 0:
		usage()
	
	tasker=Tasker()
	tasker.uploadChanges( poollist )


if __name__ == "__main__" :
	if len( sys.argv ) == 1:
		usage()

	try:
		if sys.argv[1] == "do":
			TaskerDo( sys.argv )
		elif sys.argv[1] == "setparameters":
			TaskerSetparameters( sys.argv )
		elif sys.argv[1] == "processtask":
			TaskerProcesstask( sys.argv )
		elif sys.argv[1] == "runannotator":
			TaskerRunannotator( sys.argv )
		elif sys.argv[1] == "upload":
			TaskerUpload( sys.argv )
		else:
			usage()
	except TaskerError, err:
		print err
