class InstallDirs :
	
	def __init__(self) :
		self.prefix = ''
		self.lib = ''
		self.bin = ''
		self.inc = ''
		self.data = ''
	
	def compose( self, environ ) :
		self.prefix = environ['prefix']
		self.lib =  environ['prefix']+'/lib'
		self.bin    = environ['prefix']+'/bin'
		self.inc    = environ['prefix']+'/include'
		self.data   = environ['prefix']+'/share'

