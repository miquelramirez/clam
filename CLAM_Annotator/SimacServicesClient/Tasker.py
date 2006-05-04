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
		self.printfunction = printfunction
		self.config = shelve.open( 'config.dict', writeback=True)

	def __del__( self ):
		self.config.close()

	def processTask( self, taskfile, projectname, path=os.getcwd() ):
		path += os.sep
		self.config['taskfile'] = taskfile
		self.config['projectname'] = projectname
		self.config['path'] = path 
		self.config['songlisting'] = {}
		self.config['modifydescriptors'] = []

		task = self._retrieveTask( taskfile )

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

		os.utime( taskfile, None )


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
			count=0
			for song in songlisting.keys():
				#if it has been modified
				if song in modifiedlist:
					count += 1
					self.printfunction( u"\n - Packing descriptors of: %s\n" % song )
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

			if count==0:
				self.printfunction( u" - No valid song descriptor has been found. Not doing any upload\n" )
				return
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


	def clean( self ):
		try:
			taskfile = self.config['taskfile']
			projectname = self.config['projectname']
			path = self.config['path']
			songlisting = self.config['songlisting']
		except KeyError:
			raise TaskerError( "Workflow error\nCould not restore configuration, make sure you follow the usage process." )

		tryremove = lambda name : os.path.exists( name ) and os.remove( name )

		tryremove( path+os.sep+projectname+".sc" )
		tryremove( path+os.sep+projectname+".pro" )
		tryremove( taskfile )
		for song in songlisting.keys():
			tryremove( path+os.sep+song )
			tryremove( path+os.sep+song+".pool" )
		self.config.close()
		tryremove( "config.dict" )


	def _retrieveTask( self, location ):
		try:
			return FromXmlStream( location )
		except:
			raise TaskerError( "Task file error\nReading file '%s'. It doesn't exist or it is a malformed XML file." % location )


	def _downloadSong( self, locations, savetopath ):
		#EP: quan es fagi servir el ContentProvider, el nom d'arxiu sera un problemet, diria -> caldria mirar header pel nom d'arxiu?
		if len( locations ) == 0:
			return None

		for url in locations.splitlines():
			try:
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


	def _createFile( self, path, name, extension, content ):
		try:
			file = open( path+name+extension, 'w' )
			file.write( content )
			file.close()
		except:
			raise TaskerError( "File write error\nError writing file '%s', maybe the directory does not exist!" % ( name+extension ) )

