import jack

def print_time ():
    print ("current time: %f s" % (
        float(jack.get_current_transport_frame())/jack.get_sample_rate() 
                    )
          )

#testing position set/get
from time import sleep
jack.attach("/var/run/jack")
print ("getting current time")
print_time()
print ("going to frame 0")
jack.transport_locate(0)
sleep (0.2)

print ("getting current time")
print_time()

sleep (0.2)
print ("going to 6 sec")
jack.transport_locate(jack.get_sample_rate()*6)
sleep (0.2)

print ("getting current time")
print_time()

#testing state set/get
def print_state():
    state = jack.get_transport_state()
    statename = {
            jack.TransportStopped:"stopped",
            jack.TransportRolling:"rolling",
            jack.TransportStarting:"starting"
        } [state]
    print("current state is %i (%s)"% (state, statename))

print ("TransportStopped: %i" % jack.TransportStopped)
print ("TransportRolling: %i" % jack.TransportRolling)
print ("TransportStarting: %i" % jack.TransportStarting)
print_state()

print ("playing")
jack.transport_start()
sleep(5)
print ("stopping")
jack.transport_stop()

print_time()
