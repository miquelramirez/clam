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
import sys, time, os
import jack

# Prototyper needs at least 1 sec
run_client_wait_time = 0.1
connect_wait_time = 0.01

# private interface:

def _client_from_port(client) :
	return client.split(':')[0]

def _ports_of_client(client) :
	ports = jack.get_ports()
	return [port for port in ports if _client_from_port(port) == client]

def connect(source, target) :
	os.system('jack_connect %s %s' % (source, target))
	time.sleep( connect_wait_time )

def _init() :
	try :
		jack.attach('jack_bus_connect')
	except jack.NotConnectedError:
		print "jackd not running"
		sys.exit()
def _exit() :
	jack.detach()
	
# public interface :

def outports(client) : #TODO assert is string
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if jack.get_port_flags(port) & jack.IsOutput ]

def inports(client) :
	client_ports = _ports_of_client(client)
	return [port for port in client_ports if jack.get_port_flags(port) & jack.IsInput ]

def bus_connect(source, target) :
	"""
	Connects two lists of ports. The arguments can be a list or a string. If the latter, all the available ports of the client will be used.
	"""
	sources = source if type(source) == type([]) else outports(source)
	targets = target if type(target) == type([]) else inports(target)
	num_connections = min(len(sources), len(targets))
#	print 'Doing %i connections. Client "%s" has %i out ports and "%s" has %i in ports' % (num_connections, _client_from_port(sources[0]), len(sources), _client_from_port(targets[0]), len(targets))
	print 'Doing %i connections. Client has %i out ports and target has %i in ports' % (num_connections, len(sources), len(targets))
	for i in xrange(num_connections) :
		print 'connect', sources[i], targets[i]
		connect(sources[i], targets[i])

def clients() :
	ports = jack.get_ports()
	l = [] 
	l += set( [_client_from_port(port) for port in ports] )
	return l

def get_added_client(old_clients, new_clients) :
	l = []
	l += set(new_clients) - set(old_clients)
	if len(l) != 1 :
		print 'WARNING expected strictly one added jack client, found', len(l)
		return ""
	print l
	return l[0]

def run_jack_client(cmd, wait=run_client_wait_time) :
	previous_clients = clients()
	os.system( cmd+'&' )
	time.sleep( wait )
	current_clients = clients()
	return get_added_client(previous_clients, current_clients)

def kill_jack_client(cmd) :
	program = cmd.split()[0]
	os.system( 'pkill '+program )


def main() :

	metro = 'jack_metro -b 60'
	c1 = run_jack_client(metro)
	c2 = run_jack_client(metro)
	c3 = run_jack_client(metro)

	bus_connect(c1, 'system')
	bus_connect('system', 'system')

	bus_connect( outports('system')[:-1] , inports('system')[-2:] )
	bus_connect( c2 , inports('system')[-3:] )

	time.sleep(4)
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
	jack.check_events() #TODO what this does?
	jack.activate()
	jack.deactivate()
'''
