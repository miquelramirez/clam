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


if __name__ == "__main__" :
	unittest.main()

