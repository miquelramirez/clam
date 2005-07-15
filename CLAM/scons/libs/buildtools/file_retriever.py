import re, os, glob

class FileRetriever :
	__hdr_extensions = ['h', 'H', 'hxx', 'hpp']
	__src_extensions = ['c', 'C', 'cxx', 'cpp', r'c\+\+', 'cc']


	def __init__(self, basedir, folders, blacklist  ) :
		self.out_inc = 'include/CLAM'
		self.out_src = 'src'
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

	def scan( self, header_db = None ) :
		for target in self.scantargets :
			if not os.path.isdir( target ) :
				base = os.path.dirname(target)
				for file in glob.glob(target ) :
					if not self.is_blacklisted(file) :
						if self.is_header(file) :
							os.system( 'cp %s %s'%(file,self.out_inc) )
							self.headers.append( '%s/%s'%(self.out_inc,os.path.basename(file)) )
						if self.is_source(file) :
							os.system( 'cp %s %s'%(file,self.out_src) )
							self.sources.append( '%s/%s'%(self.out_src,os.path.basename(file)) )
			else :

				for file in os.listdir(target) :
					if not self.is_blacklisted(file) :
						if self.is_header(file) :
							os.system( 'cp %s/%s %s'%(target,file,self.out_inc) )
							self.headers.append( '%s/%s'%(self.out_inc,file) )
						if self.is_source(file) :
							os.system( 'cp %s/%s %s'%(target,file,self.out_src) )
							self.sources.append( '%s/%s'%(self.out_src,file) )

			
