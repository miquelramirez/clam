- General controls
	# with an implicit network would be free functions.
	network.load("filename.clamnetwork")
	network.import("<Network> .... </Network>")
	network.save()
	network.save("filename")

- Transport
	# with an implicit network would be free functions.
	network.play()
	network.stop()
	network.pause()
	network.isStopped()
	network.isPlaying()
	network.isReady()
	network.whyIsNotReady # TOREVIEW

- Processings
	proc = network.add("ProcessingType")
	proc = network.add("name", "ProcessingType")
	proc = add("ProcessingType") # implicit network
	proc = add("name", "ProcessingType") # implicit network
	network.remove("name")
	remove(proc) # implicit network alternative
	proc.remove() # alternative
	ports = proc._outports  # connections is an iterable
	ports = proc._inports  # connections is an iterable
	controls = proc._outcontrols  # connections is an iterable
	controls = proc._incontrols  # connections is an iterable
	proc.rename("newname")
	proc2 = proc.clone()
	proc2 = proc.clone("name2")

- Processing Configuration
	# Changes on proc._config has no effect on the processing 
	# until 'reconfigure' is called
	config = Config("ProcessingType")
	proc.ConfigField = newvalue
	proc["ConfigField"] = newvalue
	proc._config["ConfigField"] = newvalue
	proc._config.ConfigField = newvalue
	# analog for getters
	proc.updateConfig(
		ConfigField = value1,
		ConfigField2 = value2,
	)
	proc.config=config
	proc.reconfigure()
	proc.configured

- Obtaining ports/controls
	proc.Output
	proc['Output']
	proc._outports['Output']
	proc._outports.Output
	proc._outports[4] # Zero based to be consistent with python indexing
	proc.Output.type  # not sure what to return
	proc.Output.name
	proc.Output.index
	proc.Output.peers # returns a list of connected peers

- Connecting
	proc._outports.connect(proc2) # Connects all feasible ports
	proc._outcontrols.connect(proc2) # Connects all feasible controls
	proc > proc2        # shorthand for ._outports.connect
	proc.Output.connect(proc2.Input)
	proc.Output > proc2.Input      # shorthand for .connect

- Disconnect controls/ports
	proc.Output.disconnect(proc2.Input)
	proc.Output.disconnect() # any connected
	proc.Input.disconnect() # any connected
	for port in proc._inports : port.disconnect()