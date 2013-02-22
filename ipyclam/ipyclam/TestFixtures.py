import Dummy_NetworkProxy

def empty() :
	return Dummy_NetworkProxy.Dummy_NetworkProxy([], [], [])


def dummyConfigWithStrings() :
	return dict(
		ConfigParam1 = "Param1",
		ConfigParam2 = "Param2",
		ConfigParam3 = "Param3"
	)

def dummyConfigWithNestedConfigs() :
	return dict(
		ConfigParam1 = "default1",
		ConfigParam2 = "default2",
		ConfigParam3 = dict(
			NestedParam1 = "defaultnested1",
			NestedParam2 = "defaultnested2"
		)
	)
