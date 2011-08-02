#!/usr/bin/python

from PyQt4 import QtGui, QtCore
import sys
import ipyclam

class ipyclam_controller(QtGui.QWidget):
	def __init__(self):
		super(ipyclam_controller, self).__init__()
		self.createNetwork()

		hbox = QtGui.QHBoxLayout()
		button = QtGui.QPushButton("Play")
		self.connect(button, QtCore.SIGNAL('clicked()'), self.play)
		hbox.addWidget(button)

		button = QtGui.QPushButton("Pause")
		self.connect(button, QtCore.SIGNAL('clicked()'), self.pause)
		hbox.addWidget(button)

		button = QtGui.QPushButton("Stop")
		self.connect(button, QtCore.SIGNAL('clicked()'), self.stop)
		hbox.addWidget(button)

		button = QtGui.QPushButton("Code")
		self.connect(button, QtCore.SIGNAL('clicked()'), self.printCode)
		hbox.addWidget(button)

		self.setLayout(hbox)

	def createNetwork(self):
		self._network = ipyclam.Network( ipyclam.Clam_NetworkProxy() )
		# Processings
		self._network.source = "AudioSource"
		self._network.sink = "AudioSink"
		self._network.smsanalysis = "SMSAnalysisCore"
		self._network.smssinusoidalgain = "SMSSinusoidalGain"
		self._network.smsresidualgain = "SMSResidualGain"
		self._network.smsfreqshift = "SMSFreqShift"
		self._network.synthesizer = "SMSSynthesis"
		self._network.outcontrolsender = "OutControlSender"
		# Connections
		self._network.source > self._network.smsanalysis
		self._network.smsanalysis["Sinusoidal Peaks"] > self._network.smssinusoidalgain["In SpectralPeaks"]
		self._network.smsanalysis > self._network.smsresidualgain
		self._network.smssinusoidalgain > self._network.smsfreqshift
		self._network.smsfreqshift > self._network.synthesizer
		self._network.smsresidualgain > self._network.synthesizer
		self._network.synthesizer.OutputAudio > self._network.sink["1"]
		self._network.outcontrolsender > self._network.smsfreqshift

	def printCode(self):
		print self._network.code()

	def play(self):
		self._network.play()
		print 'Playing...'

	def pause(self):
		self._network.pause()
		print 'Paused'

	def stop(self):
		self._network.stop()
		print 'Stopped'

if __name__ == "__main__" :
	app = QtGui.QApplication(sys.argv)
	w = ipyclam_controller()
	w.show()
	app.exec_()
