import re
import string

class Parser:
	_startCountingSymbol="EfficiencyTest::start()"
	_stopCountingSymbol="EfficiencyTest::stop()"
	_startCountingId = 0	
	_stopCountingId = 0
	_foundLimits = 0
	_functionValues = []
	_symbolRe = r'^c?fn=\((?P<id>\d+)\) (?P<symbol>%s)\n'
	_startCountingRe = re.compile(_symbolRe % _startCountingSymbol)
	_stopCountingRe = re.compile(_symbolRe % _stopCountingSymbol)
	_testMethodRe = re.compile(_symbolRe % "[^ ]+Test::test[^ ]+")
	#_testedFunctionsIdList = []
	#_cfnMap = {}
	

	## 1st Parse
	def ExtractSymbolInformation(self,line, symbolPattern):
		""" Given a symbol pattern and a callgrind output line,
		returns a tuple of id and the symbol if there is a first
		ocurrence of the symbol. It returns None when it is not a
		first symbol usage or when the pattern doesn't match.
		"""
		match = re.compile(r'^c?fn=\((?P<id>\d+)\) (?P<symbol>'+symbolPattern+')\n').search(line)
		if (match == None): return None
		return (int(match.group('id')), match.group('symbol'))

				
	def FindLimitsId(self,line):
		"""Given a line to check saves the id of the symbols for
		start and stop simbols if they are included.
		Returns 1 when any of them are found and None when not.
		"""
		pair = self.ExtractSymbolInformation(line,".*")
		if pair==None:
			return None
		
		if pair[1]==self._startCountingSymbol:
			self._startCountingId = pair[0]
			self._foundLimits = self._foundLimits+1;
			return 1
			
		if pair[1]==self._stopCountingSymbol:
			self._stopCountingId = pair[0]
			self._foundLimits = self._foundLimits+1;
			return 1

	def BuildSymbolMap(self,stream, symbolPattern) :
		"""Build the symbol map.
		"""
		symbolMap = {}

		for line in stream:
			pair = self.ExtractSymbolInformation(line, symbolPattern)
			if pair != None:
				symbolMap[pair[0]] = pair[1]
				continue
			
			if self._foundLimits!=2: self.FindLimitsId(line)
		
		return symbolMap
	
	###2nd Parse	
	def IsStart(self,line):
		match = re.compile(r'^cfn=\((?P<id>\d+)\)').search(line)
		if match  ==  None: return 0
		functionId = int(match.group('id'))
		if functionId != self._startCountingId: return 0
		return 1		
		
	def IsStop(self,line):
		match = re.compile(r'^cfn=\((?P<id>\d+)\)').search(line)
		if match  ==  None: return -1
		functionId = int(match.group('id'))
		if functionId != self._stopCountingId: return 0
		return 1		
	
	def createFunctionName(self,functionNumber):
		decimal = "";
		if functionNumber<10: decimal = "0";
		return decimal+str(functionNumber);

	def takeFunctionValues(self,stream):
##		print 'taking function values'
		self._functionValues = []
		for line in stream:
##			print line        
			if re.compile("([\*] |[\+-]?\d+ )+").search(line) == None:
				return line
			lineValues = re.compile("([\*]|[\+-]?\d+)").findall(line)
			self._functionValues.append(lineValues)		
##		print 'end taking values'
		return line
	
	def BuildTestedFunctionsMap(self,stream):
##		print 'building tested function map'
		functionNumber = 0
		flag=0
		testedFunctionsMap={}
		for line in stream:
##			print line        
			match = re.compile(r'^cfn=\((?P<id>\d+)\)').search(line)
			if match  ==  None: continue
			
			functionId = int(match.group('id'))
			if functionId == self._stopCountingId: 
				return testedFunctionsMap
			
			while flag==0:
				functionNumber = functionNumber+1
				functionName=self.createFunctionName(functionNumber)
				flag=self.IsStop(self.takeFunctionValues(stream))
				testedFunctionsMap[functionName] = self._functionValues
				if flag==1: return testedFunctionsMap
			
		return None

	def FindStartCounting(self,stream):
##		print 'find start counting'
		for line in stream:
##			print line        
			if re.compile("cfn=\("+str(self._startCountingId)+"\)").search(line) != None: return 1
		return 0
	
	
	def FindTest(self,stream,symbolMap):
##		print 'Finding test'
		for line in stream:
			match = re.compile(r'^fn=\((?P<id>\d+)\)').search(line)
			if match  ==  None: continue
			functionId = int(match.group('id'))
			if symbolMap.has_key(functionId): return functionId
			
		return 0
		
		
	def BuildTestMap(self,stream,symbolMap):
		testedFunctionsMap = {}
		TestMap = {}
##		print self._startCountingId
##		print self._stopCountingId        

		while 1:
			functionId = self.FindTest(stream,symbolMap)
			if functionId == 0: continue
##			print functionId           
			functionName = symbolMap[functionId]
			del symbolMap[functionId]
			if self.FindStartCounting(stream) == 0: break
			testedFunctionsMap = self.BuildTestedFunctionsMap(stream)
			if testedFunctionsMap == None: break
			TestMap[functionName] = testedFunctionsMap
			if symbolMap == {}:break
		return TestMap		


	#Parsing Complete

	def completeParsing(self,stream):
		symbolMap=self.BuildSymbolMap(stream,"[^ ]+Test::test[^ ]+") 
##		print symbolMap
		stream.seek(0)
		testMap=self.BuildTestMap(stream,symbolMap)
##		print testMap
		return testMap

