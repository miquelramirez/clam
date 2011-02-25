class ProcessingSymbols(object):
	def __init__(self):
		self._dict = dict(
			MinimalProcessing = dict(
				type = "MinimalProcessing",
				config = dict(),
				inports = [],
				outports = [],
				incontrols = [],
				outcontrols = []
			),
			PortSink = dict(
				type = "PortSink",
				config = dict(),
				inports = [['InPort1', 'DataType']],
				outports = [],
				incontrols = [],
				outcontrols = []
			),
			PortSource = dict(
				type = "PortSource",
				config = dict(),
				inports = [],
				outports = [['OutPort1', 'DataType']],
				incontrols = [],
				outcontrols = []
			),
			ControlSink = dict(
				type = "ControlSink",
				config = dict(),
				inports = [],
				outports = [],
				incontrols = [['InControl1', 'ControlType']],
				outcontrols = []
			),
			ControlSource = dict(
				type = "ControlSource",
				config = dict(),
				inports = [],
				outports = [],
				incontrols = [],
				outcontrols = [['OutControl1', 'ControlType']]
			),
			OtherControlSink = dict(
				type = "OtherControlSink",
				config = dict(),
				inports = [],
				outports = [],
				incontrols = [['InControl1', 'OtherControlType']],
				outcontrols = []
			)
		)
	def __getattr__(self, name) :
		if name not in self._dict:
			raise AttributeError(name)
		return self._dict[name]
	def __dir__(self) :
		return self._dict.keys()

import unittest

class ProcessingSymbolsTests(unittest.TestCase):
	def test_ProcessingGettingAsAttribute(self) :
		symbol = ProcessingSymbols()
		self.assertEqual("MinimalProcessing", symbol.MinimalProcessing["type"])
	def test_BadProcessingGettingAsAttributeAndFail(self) :
		symbol = ProcessingSymbols()
		try:
			symbol.BadProcessingType
		except AttributeError, e:
			self.assertEqual(("BadProcessingType",), e.args)
	def test_dir(self) :
		symbol = ProcessingSymbols()
		self.assertEqual(["ControlSink", "ControlSource", "MinimalProcessing", "OtherControlSink", "PortSink", "PortSource"], dir(symbol))

if __name__ == "__main__":
	unittest.main()

