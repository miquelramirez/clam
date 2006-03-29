import sys
import StringIO
from DataCollector import *

print "We are going to Execute the callgrind collector :)"
input = sys.argv[1]               
inputFile=open(input,'r')
dataCollector=DataCollector()
dataCollector.testMapToDataBase(inputFile)
