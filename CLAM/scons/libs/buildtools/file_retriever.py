import re, os, glob

class FileRetriever :
	__hdr_extensions = ['h', 'H', 'hxx', 'hpp']
	__src_extensions = ['c', 'C', 'cxx', 'cpp', r'c\+\+', 'cc']


	def __init__(self, basedir, folders, blacklist  ) :
		self.headerREs = []
		
		for ext in self.__hdr_extensions :
			self.headerREs.append( re.compile( r'^.+\.%s$'%ext ) )

		self.sourceREs = [] 

		for ext in self.__src_extensions : 
			self.sourceREs.append( re.compile( r'^.+\.%s$'%ext  ) )
	
		self.scantargets = [ basedir+'/'+folder for folder in folders ]

		self.blacklisted = []

		for entry in blacklist :
			self.blacklisted.append( re.compile( entry ) )

		self.headers = []
		self.sources = []

	def is_blacklisted( self, filename ) :
		for entry in self.blacklisted :
			if entry.search(filename) is not None :
				return True		

		return False

	def is_header( self, filename ) :
		for regexp in self.headerREs :
			if regexp.search( filename ) is not None :
				return True
		return False

	def is_source( self, filename ) :
		for regexp in self.sourceREs :
			if regexp.search( filename ) is not None :
				return True
		return False

	def scan( self ) :
		for target in self.scantargets :
			if not os.path.isdir( target ) :
				base = os.path.dirname(target)
				for file in glob.glob(target ) :
					if not self.is_blacklisted(file) :
						if self.is_header(file) :
							os.system( 'cp %s include/CLAM'%(file) )
							self.headers.append( 'include/CLAM/%s'%os.path.split(file)[1] )
						if self.is_source(file) :
							os.system( 'cp %s src'%(file) )
							self.sources.append( 'src/%s'%os.path.split(file)[1] )
			else :

				for file in os.listdir(target) :
					if not self.is_blacklisted(file) :
						if self.is_header(file) :
							os.system( 'cp %s/%s include/CLAM'%(target,file) )
							self.headers.append( 'include/CLAM/%s'%file )
						if self.is_source(file) :
							os.system( 'cp %s/%s src'%(target,file) )
							self.sources.append( 'src/%s'%file )

			
