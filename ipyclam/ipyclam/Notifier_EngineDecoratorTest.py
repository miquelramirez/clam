from Notifier_EngineDecorator import Notifier, Notifier_EngineDecorator, notified
import unittest


class NotifierTest(unittest.TestCase) :

	class Adaptee(object) :
		def __init__(self) : pass
		def setter(self, value) : return "Setting " + value
		def getter(self) : return "Getting"

	class Notifier(Notifier) :
		def __init__(self, adaptee) :
			super(NotifierTest.Notifier, self).__init__(adaptee)
			self._notified = False

		@notified
		def setter(self, value) :
			return ""

		def notify(self) :
			self._notified = True

	def testGetter(self) :
		o  = self.Notifier(self.Adaptee())
		o.setCallback(o.notify)
		result = o.getter()
		self.assertEqual(result, "Getting")
		self.assertFalse(o._notified)

	def testSetter(self) :
		o  = self.Notifier(self.Adaptee())
		o.setCallback(o.notify)
		result = o.setter("param")
		self.assertEqual(result, "Setting param")
		self.assertTrue(o._notified)

	def testSetter_withNoCallback(self) :
		o  = self.Notifier(self.Adaptee())
		result = o.setter("param")
		self.assertEqual(result, "Setting param")
		self.assertFalse(o._notified)

import Network

class Notifier_ConfigEngineDecoratorTest(unittest.TestCase) :
	def setUp(self) :
		self.reset()
		self.net = Network.Network()
		self.net._engine.setCallback(self.callback)

	def callback(self) :
		self.notified = True

	def reset(self) :
		self.notified = False

	def test_moduleCreated(self) :
		self.net.source = "AudioSource"
		self.assertTrue(self.notified)

	def test_resetAfterModuleCreated(self) :
		self.net.source = "AudioSource"
		self.reset()
		self.assertFalse(self.notified)

	def test_getterDoNotNotify(self) :
		self.net.source = "AudioSource"
		self.reset()
		self.net.source._inports
		self.assertFalse(self.notified)

	def test_configNotifies(self) :
		self.net.source = "AudioSource"
		self.reset()
		with self.net.source._config as c :
			c.NSources = 22
		self.assertTrue(self.notified)













if __name__ == "__main__" :
	unittest.main()

