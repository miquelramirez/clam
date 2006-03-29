import yaml
import re

class FileEditor:
	
	def assignNames(self,valuesList):
		
		return {
				'Instruction Searching':valuesList[0],
				'Reading Access':valuesList[1],
				'Writing Access':valuesList[2],
				'Searching Error':valuesList[3],
				'Reading Error':valuesList[4],
				'Writing Error':valuesList[5],
				'Cicle Estimation':valuesList[6]
								
			}
	
	def modifyValuesList(self,costMap):
		
		keyList=costMap.keys()
		keyListLength=len(keyList)
		i=0;
		while i<keyListLength:
			costMap[keyList[i]]=self.assignNames(costMap[keyList[i]])
			i=i+1
		
		return costMap
		
	def mapToFile(self,costMap):
		#file = open('/root/Desktop/framework/testfileeditor/OutputFukle.yml', 'w')
		file=open('YamlFile.yml','w')
		keyList=costMap.keys()
		keyListLength=len(keyList)
		i=0;
		while i<keyListLength:
			yaml.dumpToFile(file,{keyList[i]:costMap[keyList[i]]})
			i=i+1
			
		file.close()
		
		
	def buildEventsMap(self,stream):
		eventsMap={}
		eventExpr=re.compile(r'[ ]+(?P<name>[\w ]+): (?P<value>\d+)\n')
		for line in stream: 
			match=eventExpr.search(line)
			if line=="---\n": return eventsMap
			if match==None: continue
			eventsMap[match.group('name')]=int(match.group('value'))
		
		return eventsMap
		
	def fileToMap(self,stream):
		testMap={}
		testNameExpr= re.compile(r'\'(?P<name>[^ ]+Test::test[^ ]+)\':\n')
		
		for line in stream: 
			match=testNameExpr.search(line)
			if match==None: continue
			testMap[match.group('name')]=self.buildEventsMap(stream)
		
		return testMap
			
	
	
