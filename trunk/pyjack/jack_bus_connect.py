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
	ports = monitor_client.get_ports()
	return [port for port in ports if _client_from_port(port) == client ]

def _get_ports_as_list(source, target):
	sources = source if type(source) == type([]) else outports(source)
	targets = target if type(target) == type([]) else inports(target)
	num_connections = min(len(sources), len(targets))
	return sources, targets, num_connections

def connect(source, target) :
	#print(source, target) 
	monitor_client.connect(source,target)
	time.sleep( connect_wait_time )

def disconnect(source, target) :
	#print(source, target) 
	monitor_client.disconnect(source,target)
	time.sleep( connect_wait_time )

def _init() :
	try :
		global monitor_client
		monitor_client = jack.Client('jack_bus_connect')
	except jack.NotConnectedError:
		print "jackd not running"
		sys.exit()

def _exit() :
	monitor_client.detach()
	
# public interface :

def outports(client) : #TODO assert is string
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if monitor_client.get_port_flags(port) & jack.IsOutput ]

def inports(client) :
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if monitor_client.get_port_flags(port) & jack.IsInput ]


def bus_connect(source, target, wait=max_tries_in_seconds) :
	"""
	Connects two lists of ports. The arguments can be a list or a string. 
	If the latter, all the available ports of the client will be used.
	"""
	sources, targets, num_connections = _get_ports_as_list(source, target)

#	print 'Doing %i connections. Client "%s" has %i out ports and "%s" has %i in ports' % \
#		(num_connections, _client_from_port(sources[0]), len(sources), _client_from_port(targets[0]), len(targets))
	print 'Doing %i connections. Client has %i out ports and target has %i in ports' \
		% (num_connections, len(sources), len(targets))
	
	tries=0
	max_tries = float(wait) / run_client_wait_time
	for i in xrange(num_connections) :
		while len(monitor_client.get_connections(sources[i])) == 0 and tries < max_tries:
			connect(sources[i], targets[i])
			tries += 1

def bus_disconnect(source, target) :
	sources, targets, num_connections = _get_ports_as_list(source, target)

	print 'Doing %i disconnects. Client has %i out ports and target has %i in ports' \
		% (num_connections, len(sources), len(targets))
	
	for i in xrange(num_connections) :
			disconnect(sources[i], targets[i])

def clients() :
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

def run_jack_client(cmd, wait=max_tries_in_seconds) :
	previous_clients = clients()
	pid = subprocess.Popen(cmd.split(' '), stdout=subprocess.PIPE, env=os.environ)
	new_client = ""
	tries=0
	max_tries = float(wait) / run_client_wait_time
	while new_client == "" and tries < max_tries:
		time.sleep(run_client_wait_time * wait)
		new_client = get_added_client(previous_clients, clients())
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
	_init()
	main()
	_exit()
elif __name__ == 'jack_bus_connect' :
	_init()

	
'''
not used:
	monitor_client.check_events() #TODO what this does?
	monitor_client.activate()
	monitor_client.deactivate()
'''
