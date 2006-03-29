import yaml
import datetime
import sys
from Parser import *
from Cost import *
from FileEditor import *
#from DBImporter import *
#from DBEditor import *


class DataCollector:

    _server="localhost"
    _user="blashyrk"
    _password="wacken4ever"
    _dataBase="testsDB"
    
    def parsing(self,inputFile):
        parser=Parser()
        parser._testedFunctionsIdList=[]
        testMap=parser.completeParsing(inputFile)
        return testMap
        
    def buildCostMap(self,testMap):
        cost=Cost()
        costMap=cost.buildCostMap(testMap)
        return costMap
        
    def createYamlFile(self,costMap):
        fileEditor=FileEditor()
        costmap=fileEditor.modifyValuesList(costMap)
        fileEditor.mapToFile(costMap)
        
    def yamlToMap(self):
        fileEditor=FileEditor()
        yamlFile=open('YamlFile.yml','r')
        testMap=fileEditor.fileToMap(yamlFile)
        return testMap
    
    
    def testMapToDataBase(self,inputFile):
##        dbi=DBImporter()
##        dbe=DBEditor(self._server,self._user,self._password,self._dataBase)
        testingDate = datetime.date.today().strftime("%Y%m%d")
        
        testMap=self.parsing(inputFile)
        print "Test Map:"
        print testMap
        print "  "
        
        costMap=self.buildCostMap(testMap)
        print "Cost Map:"
        print costMap
        
        self.createYamlFile(costMap)
        yamlFile=open('YamlFile.yml','r')
        print yamlFile.readlines()
        yamlFile.close()


##        testMap=self.yamlToMap()
##        
##        dbi.importDate(testingDate,dbe)
##        return dbi.costMapToDB(testMap,testingDate,dbe)
        

##print "We are Executing the callgrind collector :)"
##input = sys.argv[1]               
##inputFile=open(input,'r')
##dataCollector=DataCollector()
##dataCollector.testMapToDataBase(inputFile)
	

