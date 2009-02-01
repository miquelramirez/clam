# -*- coding: UTF-8 -*-
# Todo: the solution of temporary files is ugly!
#       Boost.python should be considered later so that the data type conversion to C++ could be easier
#       "Python Dictionary" should be perfect
import urllib2
import time
import re
import socket

from AudioIDLink import *
from urllib2 import Request, urlopen, URLError, HTTPError

sparqlFront='http://dbtune.org/jamendo/sparql/?query=describe%20%3C'
sparqlEnd='%3E'
jamendoFront='http://www.jamendo.com/en'

#set socket timeout
timeout=30
socket.setdefaulttimeout(timeout)

class SemWebCrawler:
	def __init__(self, filename) :
		self.filename = filename
		self.editoralData = ()


	def _tempRecord(self,filename,data):
		op = open(filename, "wb")
		for str in data:
			op.write(str)
			op.write('\n')
		op.close()
		return 1
	
	def _requestServer(self, url):
		req = urllib2.Request(url)
		time.sleep(3)
		try:
			fp=urllib2.urlopen(req)
		except HTTPError, e:
			print 'The server ',url,' couldn\'t fulfill the request.'
			print 'Error code:',e.code
			return ' '
		except URLError,e:
			print 'We failed to reach the server',url
			print 'Reason:', e.reason
			return ' '
		try:
			s = fp.read()
		except:
			print 'read HTTP exception:',url
			s=' '
		fp.close()
		return s


	def _getTrackURL(self, url):
		s=self._requestServer(url)
		pattern= re.compile("http://dbtune.org/jamendo/track/\d*")
		pattern2=re.compile("(?<=http://dbtune.org/jamendo/track/)\d*")
		if (s==' '):
			return ' ',' '
		else:
			urls=pattern.findall(s)
			url2=urls.pop(0)
			url=sparqlFront+url2+sparqlEnd  #认为第一个找到的url即目标track
			urls=pattern2.findall(url2)
			url2=jamendoFront+'/track/'+urls.pop(0)
			return url, url2

	def _getRecordURL(self, trackUrl, trackUrl2, trackUrl3):
		trackData = []
		if (trackUrl!=' '):
			s=self._requestServer(trackUrl)
			if (s!=' '):
				pattern = re.compile("http://dbtune.org/jamendo/record/\d*")
				pattern2 = re.compile("(?<=http://dbtune.org/jamendo/record/)\d*")
				titlePattern = re.compile("(?<=<dc:title rdf:datatype=\"&xsd;string\">).*(?=</dc:title>)")
				trackNumberPattern = re.compile("(?<=<mo:track_number rdf:datatype=\"&xsd;int\">).*(?=</mo:track_number>)")
				urls = pattern.findall(s)
				url2 = urls.pop(0)
				url = sparqlFront+url2+sparqlEnd  #认为第一个找到的url即目标record
				print 'lalalallaaaaaaaaaaaaaaaa the url2= urls.pop(0)is aaaaaaaaaaaaaaaaa'
				print url2
				urls = pattern2.findall(url2)
				print urls
				url2 = jamendoFront+'/album/'+urls.pop(0)
				self._tempRecord('temp_1',titlePattern.findall(s))
				self._tempRecord('temp_2',trackNumberPattern.findall(s))
				trackData.extend(titlePattern.findall(s))
				trackData.extend(trackNumberPattern.findall(s))
				print'######the title of the track is#########'
				temp=titlePattern.findall(s)
				print temp
				print'######the number of the track is#########'
				print trackNumberPattern.findall(s)
			else:
				url = ' '
				url2 = ' '

			s2=self._requestServer(trackUrl2)
			if (s2!=' '):
				print 'trackUrl 2 is:'
				print trackUrl2
				lyricPattern = re.compile("(?<=<div id=\"lyrics\" style=\"margin-left:5px;\">)(?s).*?(?=</div>)")#with html format, 换行？(?s)
				genrePattern = re.compile("(?<=<td class='r g_genre'>).*?(?=</td>)")
				albumRatingPattern = re.compile("(?<=<td class='t'>Rating</td> <td class='s'>).*?(?=<)")
				self._tempRecord('temp_3',lyricPattern.findall(s2))
				self._tempRecord('temp_4',genrePattern.findall(s2))
				self._tempRecord('temp_5',albumRatingPattern.findall(s2))
				trackData.extend(lyricPattern.findall(s2))
				trackData.extend(genrePattern.findall(s2))
				trackData.extend(albumRatingPattern.findall(s2))
				print'######JJJJ: the lyricPattern is#########'
				print lyricPattern.findall(s2)
				print'######JJJJ: the genrePattern is#########'
				print genrePattern.findall(s2)
				print'######JJJJ: the albumRatingPattern is#########'
				print albumRatingPattern.findall(s2)
		else:
			url = ' '
			url2 = ' '

		if (trackUrl3!=' '):
			s3=self._requestServer(trackUrl3)
			if (s3!=' '):
				print 'the music URL is:'
				print trackUrl3
				musicBrainzTagPattern = re.compile("(?<=<a href=\"/show/tag/\?tag=).*?(?=&show=track\">)")
				self._tempRecord('temp_6',musicBrainzTagPattern.findall(s3))
				trackData.extend(musicBrainzTagPattern.findall(s3))
				print'######MMMM: the musicBrainzTag is#########'
				print musicBrainzTagPattern.findall(s3)
		return url,url2,trackData
	

	def _getArtistURL(self, recordUrl, recordUrl2):
		recordData = []
		if(recordUrl!=' '):
			s = self._requestServer(recordUrl)
			if (s!=' '):
				pattern= re.compile("http://dbtune.org/jamendo/artist/\d*")
				titlePattern = re.compile("(?<=<dc:title rdf:datatype=\"&xsd;string\">).*(?=</dc:title>)")
				datePattern = re.compile("(?<=dc:date=\").*?(?=\">)")
				coverjpgPattern = re.compile("(?<=<mo:image rdf:resource=\").*?(?=\"/>)")
				trackListURIPattern = re.compile("(?<=<mo:track rdf:resource=\").*?(?=\"/>)")
				tagPattern = re.compile("(?<=<tags:taggedWithTag rdf:resource=\"http://dbtune.org/jamendo/tag/).*?(?=\"/>)")
				urls=pattern.findall(s)
				url=sparqlFront+urls.pop(0)+sparqlEnd  #认为第一个找到的url即目标artist
				self._tempRecord('temp_7',titlePattern.findall(s))
				self._tempRecord('temp_8',datePattern.findall(s))
				self._tempRecord('temp_9',coverjpgPattern.findall(s))
				self._tempRecord('temp_10',trackListURIPattern.findall(s))
				self._tempRecord('temp_11',tagPattern.findall(s))
				recordData.extend(titlePattern.findall(s))
				recordData.extend(datePattern.findall(s))
				recordData.extend(coverjpgPattern.findall(s))
				recordData.extend(trackListURIPattern.findall(s))
				recordData.extend(tagPattern.findall(s))
				print'######the title of the record is#########'
				print titlePattern.findall(s)
				print'######the date of the record is#########'
				print datePattern.findall(s)
				print'######the jpg of the record is#########'
				print coverjpgPattern.findall(s)
				print'######the trackList of the record is#########'
				print trackListURIPattern.findall(s)
				print'######the tags of the record is#########'
				print tagPattern.findall(s)
			else:
				url = ' '
		else:
			url = ' '

		if (recordUrl2!=' '):
			s2 = self._requestServer(recordUrl2)
			if (s2!=' '):
				trackListPattern = re.compile("(?<=<td class=\"title_tracks\">).*?(?=</td>)") #the regulation match to be refined
				albumDescriptionPattern = re.compile("(?<=<div class='album_page_description'>)(?s).*?(?=</div>)")
				reviewPattern = re.compile("(?<=<div class='reviewone_text' ><p>).*?(?=</p></div>)")
				jamendoTagPattern = re.compile("(?<=class='g_tag_name'>).*?(?=</a>)")
				self._tempRecord('temp_12',trackListPattern.findall(s2))
				self._tempRecord('temp_13',albumDescriptionPattern.findall(s2))
				self._tempRecord('temp_14',reviewPattern.findall(s2))
				self._tempRecord('temp_15',jamendoTagPattern.findall(s2))
				recordData.extend(trackListPattern.findall(s2))
				recordData.extend(albumDescriptionPattern.findall(s2))
				recordData.extend(reviewPattern.findall(s2))
				recordData.extend(jamendoTagPattern.findall(s2))
				print'######JJJ: the trackList is#########'
				print trackListPattern.findall(s2)
				print'######JJJ: the albumDescription is#########'
				print albumDescriptionPattern.findall(s2)
				print'######JJJ: the review is#########'
				print reviewPattern.findall(s2)
				print'######JJJ: the jamendoTag is#########'
				print jamendoTagPattern.findall(s2)
		return url,recordData


	def _getArtistData(self, artistUrl):
		artistData = []
		if(artistUrl!=' '):
			s = self._requestServer(artistUrl)
			if (s!=' '):
				namePattern = re.compile("(?<=>).*(?=</foaf:name>)")
				locationPattern = re.compile("(?<=<foaf:based_near rdf:resource=.).*(?=./>)")
				homepagePattern = re.compile("(?<=<foaf:homepage rdf:resource=.).*(?=./>)")
				photoPattern = re.compile("(?<=<foaf:img rdf:resource=.).*(?=./>)")
				recordListPattern = re.compile("(?<=<foaf:made rdf:resource=.).*(?=./>)")
				self._tempRecord('temp_16',namePattern.findall(s))
				self._tempRecord('temp_17',locationPattern.findall(s))
				self._tempRecord('temp_18',homepagePattern.findall(s))
				self._tempRecord('temp_19',photoPattern.findall(s))
				self._tempRecord('temp_20',recordListPattern.findall(s))
				artistData.extend(namePattern.findall(s))
				artistData.extend(locationPattern.findall(s))
				artistData.extend(homepagePattern.findall(s))
				artistData.extend(photoPattern.findall(s))
				artistData.extend(recordListPattern.findall(s))
		return artistData


	def spider(self):
		ac = AudioIDLink()
		editoralData = []
		(zitgisturi, trackUrl3) = ac.metadata(self.filename)
		trackUrl3=trackUrl3+'.html'
		startURL = sparqlFront+zitgisturi+sparqlEnd
		print '1:the startURL is..........'
		print startURL
		(trackUrl,trackUrl2)=self._getTrackURL(startURL)
		print '2:the trackURL is..........'
		print trackUrl
		(recordUrl,recordUrl2,trackData)=self._getRecordURL(trackUrl,trackUrl2,trackUrl3)
		print '3:the recordURL is..........'
		print recordUrl
		(artistUrl,recordData)=self._getArtistURL(recordUrl,recordUrl2)
		artistData=self._getArtistData(artistUrl)
		print '4:the artistURL is..........'
		print artistUrl
		editoralData.extend(trackData)
		editoralData.extend(recordData)
		editoralData.extend(artistData)
		return editoralData
		#testtest=('aa','bb','cc')
		#return testtest
		#url = 'http://dbtune.org/jamendo/sparql/?query=describe%20%3Chttp://zitgist.com/music/track/c7190fdc-72de-41b0-aa64-9fdc3bb7380e%3E'
