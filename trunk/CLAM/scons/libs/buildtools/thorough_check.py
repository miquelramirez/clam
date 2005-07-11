
class ThoroughPackageCheck :
	
	def __init__( self, name, lang, lib, test_code ) :
		self.name = name 
		self.lib = lib
		self.test_code = test_code 
		self.lang = lang
		self.test_code_extension = None
		if self.lang == 'c' :
			self.test_code_extension = '.c'
		elif self.lang == 'c++' :
			self.test_code_extension = '.cxx'
		else :
			raise RuntimeError, "%s language is not supported for specifying test code"

	def __call__( self, context, *args, **kwargs ) :
		context.Message( 'Checking that %s sample program compiles...'%self.name )
		result = context.TryCompile( self.test_code, self.test_code_extension )
		context.Result(result)
		if not result :
			return result

		context.Message( 'Checking that %s sample program links...'%self.name )
		try :
			lastLIBS = context.env['LIBS']
		except KeyError :
			lastLIBS = None
		if self.lib is not None :
			context.env.Append( LIBS=self.lib )
		result = context.TryLink( self.test_code, self.test_code_extension )
		context.Result(result)
		if not result :
			context.env.Replace( LIBS=lastLIBS )
			return result

		context.Message( 'Checking that %s sample program runs... '%self.name )
		result, errmsg = context.TryRun( self.test_code, self.test_code_extension )
		context.Result(result)		

		return result



