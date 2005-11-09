class InstallDirs :
	
	def __init__(self) :
		self.prefix = ''
		self.lib = ''
		self.bin = ''
		self.inc = ''
		self.data = ''
		self.doc = ''
	
	def compose( self, environ ) :
		if environ['install_prefix'] != '.' :
			self.prefix = environ['install_prefix']
			self.lib =  environ['install_prefix']+'/lib'
			self.bin    = environ['install_prefix']+'/bin'
			self.inc    = environ['install_prefix']+'/include'
			self.data   = environ['install_prefix']+'/share'
			self.doc    =environ['install_prefix']+'/share/doc'
		else:
			self.prefix = environ['prefix']
			self.lib =  environ['prefix']+'/lib'
			self.bin    = environ['prefix']+'/bin'
			self.inc    = environ['prefix']+'/include'
			self.data   = environ['prefix']+'/share'
			self.doc    = environ['prefix']+'/share/doc'

