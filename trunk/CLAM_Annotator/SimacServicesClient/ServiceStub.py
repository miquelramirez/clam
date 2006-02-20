import urllib, urllib2
import urlparse
import HttpFormPost

Proxies = { 'http' : 'http://proxy.upf.edu:8080', 'ftp' : 'http://proxy.upf.edu:8080' }
NoProxiesFor = ["localhost", "127.0.0.1"]
useragent = 'simac-annotator-tasker'


class ServiceStub :
	def __init__(self, serviceLocation, proxies=Proxies) :
		self.serviceUrl = serviceLocation
		self.proxies = proxies
		pass

	def remoteCall(self, serviceName, **fields):
		if urlparse.urlparse (self.serviceUrl)[1] in NoProxiesFor:
			proxy_support = urllib2.ProxyHandler( {} )
		else:
			proxy_support = urllib2.ProxyHandler( Proxies )

		opener = urllib2.build_opener( proxy_support )
		urllib2.install_opener(opener)

		try:
			content_type, body = HttpFormPost.encode_multipart_formdata_dictionary(fields)
			headers = { 'User-Agent': useragent, 'Content-Type': content_type }
			req=urllib2.Request(self.serviceUrl+"/"+serviceName, body, headers)
			result= urllib2.urlopen(req).read()
			return result
		except:
			raise "ERROR GETTING DATA FROM SERVICE"


class ContentLocator(ServiceStub) :
	def LocateId(self, id) :
		return self.remoteCall( "LocateId", id=id )

	def IdentifyUrl(self, url) :
		return self.remoteCall( "IdentifyUrl", url=url )

	def AddUrl(self, url) :
		return self.remoteCall( "AddUrl", url=url )


class MetadataProvider(ServiceStub) :
	def QueryIdByUrl(self, url) :
		return self.remoteCall( "QueryIdByUrl", url=url )

	def QuerySchema(self, descriptors) :
		return self.remoteCall( "QuerySchema", descriptors=descriptors )

	def QueryDescriptors(self, id, descriptors) :
		return self.remoteCall( "QueryDescriptors", id=id, descriptors=descriptors)

	def UploadPackedDescriptors(self, file) :
		return self.remoteCall( "UploadPackedDescriptors", packedpoolfile=open(file, 'rb') )
