#! /usr/bin/python
'''
TODO
D get jack clients
D run application in bg safely
- kill a jack client independently
D get the jack name
D encapsulate run and get jack name
D jack module bug: cannot connect to jack reusing the client name
- connect bus
- declarative clients
- declarative graph
- run graph
- existing clients (system)
- kill graph

ASSUMPTIONS
- python module gives ports in the right order
- python module doesn't introduce any overhead (why a client is needed?)
'''
import sys, time, os, subprocess
import jack

# 
max_tries_in_seconds = 5.

run_client_wait_time = 0.1
connect_wait_time = 0.01

monitor_client = None

# private interface:

def _client_from_port(client) :
	return client.split(':')[0]

def _ports_of_client(client) :
	if monitor_client == None : 
		_init(True)
	ports = monitor_client.get_ports()
	return [port for port in ports if _client_from_port(port) == client ]

def _get_ports_as_list(source, target):
	sources = source if type(source) == type([]) else outports(source)
	targets = target if type(target) == type([]) else inports(target)
	num_connections = min(len(sources), len(targets))
	if len(sources) > 0: source = _client_from_port(sources[0])
	if len(targets) > 0: target = _client_from_port(targets[0])
	return source, sources, target, targets, num_connections

def connect(source, target) :
	if monitor_client == None : 
		_init(True)
	try :
		monitor_client.connect(source,target)
	except jack.UsageError :
		return ""
	time.sleep( connect_wait_time )
	connected_in_ports = monitor_client.get_connections(source)
	return target in connected_in_ports

def disconnect(source, target) :
	if monitor_client == None : 
		_init(True)
	monitor_client.disconnect(source,target)
	time.sleep( connect_wait_time )

def _init(exitOnError=True) :
	try :
		global monitor_client
		monitor_client = jack.Client('jack_bus_connect')
	except jack.NotConnectedError:
		messageToPrint="jackd not running"
		if exitOnError:
			print "ERROR:",messageToPrint
			sys.exit()
		print "WARNING:",messageToPrint
		return False
	return True

def _exit() :
	if monitor_client != None : 
		monitor_client.detach()
	
# public interface :

def outports(client) : #TODO assert is string
	if monitor_client == None : 
		_init(True)
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if monitor_client.get_port_flags(port) & jack.IsOutput ]

def inports(client) :
	if monitor_client == None : 
		_init(True)
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if monitor_client.get_port_flags(port) & jack.IsInput ]


def bus_connect(source, target, wait=max_tries_in_seconds) :
	"""
	Connects two lists of ports. The arguments can be a list or a string. 
	If the latter, all the available ports of the client will be used.
	"""
	source, sources, target, targets, num_connections = _get_ports_as_list(source, target)
	print 'Doing %i connections. Client "%s" has %i out ports and "%s" has %i in ports' % \
		(num_connections,source,len(sources),target,len(targets))
	
	tries=0
	successful_connections=0
	max_tries = float(wait) / run_client_wait_time
	for i in xrange(num_connections) :
		connected=False
		while connected == False and tries < max_tries:
			connected = connect(sources[i], targets[i])
			tries += 1
		if connected == True:
			successful_connections+=1
	return successful_connections 

def bus_disconnect(source, target) :
	source, sources, target, targets, num_connections = _get_ports_as_list(source, target)
	print 'Doing %i disconnections. Client "%s" has %i out ports and "%s" has %i in ports' % \
		(num_connections,source,len(sources),target,len(targets))

	for i in xrange(num_connections) :
			disconnect(sources[i], targets[i])
	return num_connections

def clients() :
	if monitor_client == None : 
		_init(True)
	ports = monitor_client.get_ports()
	l = [] 
	l += set( [_client_from_port(port) for port in ports] )
	return l

def get_added_client(old_clients, new_clients) :
	l = []
	l += set(new_clients) - set(old_clients)
	if len(l) == 0 :
		return ""
	if len(l) > 1 :
		print 'WARNING: found more than one added client. Found ', len(l)
	return l[0]

def run_jack_client(cmd, wait=max_tries_in_seconds, path=".", env=os.environ) :
	previous_clients = clients()
	pid = subprocess.Popen(cmd, env=env, cwd=path, shell=True)
	new_client = ""
	tries=0
	max_tries = float(wait) / run_client_wait_time
	while new_client == "" and tries < max_tries:
		time.sleep(run_client_wait_time * wait)
		new_client = get_added_client(previous_clients, clients())
		print "X",
		tries += 1
	if new_client == "" :
		print "ERROR: a new client didn't show up in jackd after trying %s times." % max_tries
	return new_client, pid

def kill_jack_client(cmd) :
	program = cmd.split()[0]
	os.system( 'pkill '+program )


def main() :
	metro = 'jack_metro -b 60'
	c1 = run_jack_client(metro)
	c2 = run_jack_client(metro)
	c3 = run_jack_client(metro)

	bus_connect( outports('system')[:-1] , inports('system')[-2:] )
	bus_connect( c2 , inports('system')[-3:] )
	time.sleep(4)

	# disconnect test: first existing, then again which should not give an error
	bus_disconnect( outports('system')[:-1] , inports('system')[-2:] )
	time.sleep(.1)
	bus_disconnect( outports('system')[:-1] , inports('system')[-2:] )

	try:
		disconnect('system:capture_1','blah:playback_1')
	except Exception,e:
		print 'Expected: ', e
		
	kill_jack_client(metro) #TODO right way

print "name:", __name__
if __name__=='__main__':
	_init(True)
	main()
	_exit()
elif __name__ == 'jack_bus_connect' :
	_init(False)

	
'''
not used:
	monitor_client.check_events() #TODO what this does?
	monitor_client.activate()
	monitor_client.deactivate()
'''
