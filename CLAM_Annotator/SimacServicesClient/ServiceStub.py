import urllib, urllib2
import urlparse
import HttpFormPost

Proxies = { 'http' : 'http://proxy.upf.edu:8080', 'ftp' : 'http://proxy.upf.edu:8080' }
NoProxiesFor = ["localhost", "127.0.0.1", "10.55.0.40"]
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

	def AvailableDescriptors(self) :
		return self.remoteCall("AvailableDescriptors")

if __name__ == "__main__" :
	webservice = ServiceStub("https://localhost/SimacServices/ContentLocator")
	print webservice.remoteCall("LocateId", id="4871335", par1=1, par2="yaves")

	contentLocator = ContentLocator("https://localhost/SimacServices/ContentLocator")
	print contentLocator.LocateId(24), "should be 'NotFound'"
	print contentLocator.LocateId(4871335), "should be 'http://www.threegutrecords.com/mp3/Is This It.mp3\n'"
	print contentLocator.IdentifyUrl("http://www.threegutrecords.com/mp3/Is This It.mp3") , "should be '4871335'"
	print contentLocator.AddUrl("http://www.threegutrecords.com/mp3/Is This It.mp3"), "should be '4871335'"
	
	metadataProvider = MetadataProvider("https://localhost/SimacServices/MetadataProvider")
	print metadataProvider.QueryIdByUrl("http://www.threegutrecords.com/mp3/Is This It.mp3"), "should be '4871335'"
