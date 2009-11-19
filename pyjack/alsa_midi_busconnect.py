#!/usr/bin/python
import re
import os

def _getClients(typeOfClient):
	lines=[line.strip() for line in os.popen("aconnect -%s" % typeOfClient).readlines()]
	listOfClients=[]
	temporalList=[]
	for line in lines:
		if line.find("client ")==0:
			listOfClients.append(dict())
			listOfClients[-1]["clientName"]=re.search("'(.*)'",line).group(1)
			listOfClients[-1]["clientNumber"]=int(re.search("client (.*):",line).group(1))
			if len(temporalList)>0:
				listOfClients[-2]["ports"]=temporalList
			temporalList=[]
		else:
			line=re.search("'(.*)'",line).group(1)
			temporalList.append(line)
	listOfClients[-1]["ports"]=temporalList
	return listOfClients

def _getClientPorts(name,typeOfClient):
	listOfClients=_getClients(typeOfClient)
	index=[client['clientName'] for client in listOfClients].index(name)
	if index == -1:
		return None
	client=listOfClients[index]
	return [ "%s:%i" % (client["clientNumber"],client["ports"].index(port)) for port in client["ports"] ]

def getInputClients():
	return _getClients("i")
	
def getOutputClients():
	return _getClients("o")

def getClientInputPorts(name):
	return _getClientPorts(name,"i")

def getClientOutputPorts(name):
	return _getClientPorts(name,"o")

def bus_connect(source, target):
	"""
	Connects two lists of ports. The arguments can be a list or a string. If the latter, all the available ports of the client will be used.
	"""
"""	if type(source) == type([]):
		sources=source
	else:
		["%i:%i" % (tempSource["clientNumber"],tempSource["ports"]) getOutputClients() (source)
	targets = target if type(target) == type([]) else inports(target)
	num_connections = min(len(sources), len(targets))
#	print 'Doing %i connections. Client "%s" has %i out ports and "%s" has %i in ports' % (num_connections, _client_from_port(sources[0]), len(sources), _client_from_port(targets[0]), len(targets))
	print 'Doing %i connections. Client has %i out ports and target has %i in ports' % (num_connections, len(sources), len(targets))
	for i in xrange(num_connections) :
		print 'connect', sources[i], targets[i]
		connect(sources[i], targets[i])
"""

def main():
	print "input clients: "
	print getInputClients()
	print "output ones: "
	print getOutputClients()
	print "ardour inputs:"
	print getClientInputPorts("ardour")
	print "ardour outputs:"
	print getClientOutputPorts("ardour")

main()
