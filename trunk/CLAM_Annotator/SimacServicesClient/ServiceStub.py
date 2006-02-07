import urllib
import urlparse

Proxies = { 'http' : 'http://proxy.upf.edu:8080', 'ftp' : 'http://proxy.upf.edu:8080' }
NoProxiesFor = ["localhost", "127.0.0.1"]

class ServiceStub :
	def __init__(self, serviceLocation, proxies=Proxies) :
		self.serviceUrl = serviceLocation
		self.proxies = proxies
		pass

	def remoteCall(self, serviceName, **kwargs) :
		try :
			params=urllib.urlencode(kwargs)
			if urlparse.urlparse ( self.serviceUrl )[1] in NoProxiesFor :
				serviceResult=urllib.urlopen(self.serviceUrl+"/"+serviceName, params, {}).read();
			else :
				serviceResult=urllib.urlopen(self.serviceUrl+"/"+serviceName, params, Proxies).read();
			return serviceResult
		except:# IOError:
			raise "ERROR GETTING DATA FROM SERVICE"

class ContentLocator(ServiceStub) :
	def LocateId(self, id) :
		return self.remoteCall("LocateId", id=id)

	def IdentifyUrl(self, url) :
		return self.remoteCall("IdentifyUrl", url=url)

	def AddUrl(self, url) :
		return self.remoteCall("AddUrl", url=url)

class MetadataProvider(ServiceStub) :
	def QueryIdByUrl(self, url) :
		return self.remoteCall("QueryIdByUrl", url=url)

	def QuerySchema(self, descriptors) :
		return self.remoteCall("QuerySchema", descriptors=descriptors)

	def QueryDescriptors(self, id, descriptors) :
		return self.remoteCall("QueryDescriptors", id=id, descriptors=descriptors)
