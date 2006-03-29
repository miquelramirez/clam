import string
import re

class Cost:
	
	_Ir=1
	_Dr=2
	_Dw=3
	_I1mr=4
	_D1mr=5
	_D1mw=6
	_I2mr=7
	_D2mr=8
	_D2mw=9
	
	def fillLine(self,line):
		i=len(line)
		while i<10:
			line.append('0')
			i=i+1
		return line
		
	def assignEventsPosition(self,stream):
		
		
		expression=re.compile("events: ")
		
		for line in stream:
			if expression.search(line)!=None: break
		
		
		eventsList=re.compile("\w+").findall(line)
		self._Ir=eventsList.index('Ir')
		self._Dr=eventsList.index('Dr')
		self._Dw=eventsList.index('Dw')
		self._I1mr=eventsList.index('I1mr')
		self._D1mr=eventsList.index('D1mr')
		self._D1mw=eventsList.index('D1mw')
		self._I2mr=eventsList.index('I2mr')
		self._D2mr=eventsList.index('D2mr')
		self._D2mw=eventsList.index('D2mw')
		
		
		return 1
		
		
		
	def calculateSearchingErrors(self,line):
	#Searching errors= 10*SearchingErrorsL1 + 100*SearchingErrorsL2
		return 10*int(line[self._I1mr])+100*int(line[self._I2mr])
	
	def calculateReadingErrors(self,line):
	#Reading errors= 10*ReadingErrorsL1 + 100*ReadingErrorsL2	
		return 10*int(line[self._D1mr])+100*int(line[self._D2mr])
		
	def calculateWritingErrors(self,line):
	#Reading errors= 10*WritingErrorsL1 + 100*WritingErrorsL2	
		return 10*int(line[self._D1mw])+100*int(line[self._D2mw])
		
	def calculateCicles(self,line,totalErrors):
	#Cicles=Instructions+SearchingErrors+ReadingErrors+WritingErrors
		return 	int(line[1]) + totalErrors
	
	def calculateFunctionValuesList(self,line):
		instructions=int(line[self._Ir])
		readingAccess=int(line[self._Dr])
		writingAccess=int(line[self._Dw])
		searchingErrors=self.calculateSearchingErrors(line)
		readingErrors=self.calculateReadingErrors(line)
		writingErrors=self.calculateWritingErrors(line)
		totalErrors=searchingErrors+readingErrors+writingErrors
		cicles=self.calculateCicles(line,totalErrors)
		
		return [instructions,readingAccess,writingAccess,searchingErrors,readingErrors,writingErrors,cicles]
	
	def calculateTestValuesList(self,testedFunctionsMap):
		
		keysList=testedFunctionsMap.keys()
		testValuesList=[0,0,0,0,0,0,0]
		
		for key in keysList:
			valuesLine=self.fillLine(testedFunctionsMap[key][1])
			functionValuesList=self.calculateFunctionValuesList(valuesLine)
			i=0
			while i<7:
				testValuesList[i]=testValuesList[i]+functionValuesList[i]
				i=i+1
			
		return testValuesList
			
	def buildCostMap(self,testMap):
		keysList=testMap.keys()
		costMap={}
		
		for key in keysList:
			costMap[key]=self.calculateTestValuesList(testMap[key])
		
		return costMap
		
		
	