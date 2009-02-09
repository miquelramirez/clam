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
		i=0
		for str in data:
			op.write(str)
			i=i+1
			if (i<data.__len__()):
				op.write('\n')
		op.close()
		return 1
	
	def _requestServer(self, url):
		print url
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
		print '####################################Zitgist#######################################'
		print 'Linking to the server of \'Zitgist\':'
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
		if (trackUrl!=' '):
			print '#######################################DBTune####################################'
			print 'Linking to the server of \'DBTune\':'
			s=self._requestServer(trackUrl)
			if (s!=' '):
				pattern = re.compile("http://dbtune.org/jamendo/record/\d*")
				pattern2 = re.compile("(?<=http://dbtune.org/jamendo/record/)\d*")
				titlePattern = re.compile("(?<=<dc:title rdf:datatype=\"&xsd;string\">).*(?=</dc:title>)")
				trackNumberPattern = re.compile("(?<=<mo:track_number rdf:datatype=\"&xsd;int\">).*(?=</mo:track_number>)")
				urls = pattern.findall(s)
				url2 = urls.pop(0)
				url = sparqlFront+url2+sparqlEnd  #认为第一个找到的url即目标record
				urls = pattern2.findall(url2)
				url2 = jamendoFront+'/album/'+urls.pop(0)
				title1 = titlePattern.findall(s)
				trackNumber = trackNumberPattern.findall(s)
				self._tempRecord('temp_1', title1)
				self._tempRecord('temp_2', trackNumber)
				print'Extracting \'Track Title\':..........  ..........'
				print title1
				print'Extracting \'Track No.\':..........  ..........'
				print trackNumber
			else:
				url = ' '
				url2 = ' '

			print '#######################################Jamendo###################################'
			print 'Linking to the server of \'Jamendo\':'
			s2=self._requestServer(trackUrl2)
			if (s2!=' '):
				lyric=[]
				lyricPattern = re.compile("(?<=<div id=\"lyrics\" style=\"margin-left:5px;\">)(?s).*?(?=</div>)")#with html format, 换行？(?s)
				genrePattern = re.compile("(?<=<td class='r g_genre'>).*?(?=</td>)")
				albumRatingPattern = re.compile("(?<=<td class='t'>Rating</td> <td class='s'>).*?(?=<)")
				lyric = lyricPattern.findall(s2)
				genre = genrePattern.findall(s2)
				albumRating = albumRatingPattern.findall(s2)
				self._tempRecord('temp_3', lyric)
				self._tempRecord('temp_4', genre)
				self._tempRecord('temp_5', albumRating)
				print 'Extracting \'Lyric\':..........  ..........'
				print lyric
				print 'Extracting \'Genre\':..........  ..........'
				print genre
				print 'Extracting \'Album Rating\':..........  ..........'
				print albumRating
		else:
			url = ' '
			url2 = ' '

		if (trackUrl3!=' '):
			print '######################################Musicbrainz#################################'
			print 'Linking to the server of \'Musicbrainz\':'
			s3=self._requestServer(trackUrl3)
			if (s3!=' '):
				musicBrainzTagPattern = re.compile("(?<=<a href=\"/show/tag/\?tag=).*?(?=&show=track\">)")
				musicBrainzTag = musicBrainzTagPattern.findall(s3)
				self._tempRecord('temp_6', musicBrainzTag)
				print'Extracting \'Musicbrainz tags\':..........  ..........'
				print musicBrainzTag
		return url,url2
	

	def _getArtistURL(self, recordUrl, recordUrl2):
		if(recordUrl!=' '):
			print '#######################################DBTune####################################'
			print 'Linking to the server of \'DBTune\':'
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
				title = titlePattern.findall(s)
				date = datePattern.findall(s)
				coverjpg = coverjpgPattern.findall(s)
				trackListURI = trackListURIPattern.findall(s)
				tag = tagPattern.findall(s)
				self._tempRecord('temp_7', title)
				self._tempRecord('temp_8', date)
				self._tempRecord('temp_9', coverjpg)
				self._tempRecord('temp_10', trackListURI)
				self._tempRecord('temp_11', tag)
				print'Extracting \'record title\':..........  ..........'
				print title
				print'Extracting \'record data\':..........  ..........'
				print date
				print'Extracting \'record jpg url\':..........  ..........'
				print coverjpg
				print'Extracting \'record tracklist url\':..........  ..........'
				print trackListURI
				print'Extracting \'record tags\':..........  ..........'
				print tag
			else:
				url = ' '
		else:
			url = ' '

		if (recordUrl2!=' '):
			print '#######################################Jamendo###################################'
			print 'Linking to the server of \'Jamendo\':'
			s2 = self._requestServer(recordUrl2)
			if (s2!=' '):
				trackListPattern = re.compile("(?<=<td class=\"title_tracks\">).*?(?=</td>)") #the regulation match to be refined
				albumDescriptionPattern = re.compile("(?<=<div class='album_page_description'>)(?s).*?(?=</div>)")
				reviewPattern = re.compile("(?<=<div class='reviewone_text' ><p>).*?(?=</p></div>)")
				jamendoTagPattern = re.compile("(?<=class='g_tag_name'>).*?(?=</a>)")
				trackList = trackListPattern.findall(s2)
				albumDescription = albumDescriptionPattern.findall(s2)
				review = reviewPattern.findall(s2)
				jamendoTag = jamendoTagPattern.findall(s2)
				self._tempRecord('temp_12',trackList)
				self._tempRecord('temp_13',albumDescription)
				self._tempRecord('temp_14',review)
				self._tempRecord('temp_15',jamendoTag)
				print'Extracting \'album tracklist title\':..........  ..........'
				print trackList
				print'Extracting \'album description\':..........  ..........'
				print albumDescription
				print'Extracting \'album review\'::..........  ..........'
				print review
				print'Extracting \'jamendo tags\':..........  ..........'
				print jamendoTag
		return url


	def _getArtistData(self, artistUrl):
		if(artistUrl!=' '):
			print '#######################################DBTune####################################'
			print 'Linking to the server of \'DBTune\':'
			s = self._requestServer(artistUrl)
			if (s!=' '):
				namePattern = re.compile("(?<=>).*(?=</foaf:name>)")
				locationPattern = re.compile("(?<=<foaf:based_near rdf:resource=.).*(?=./>)")
				homepagePattern = re.compile("(?<=<foaf:homepage rdf:resource=.).*(?=./>)")
				photoPattern = re.compile("(?<=<foaf:img rdf:resource=.).*(?=./>)")
				recordListPattern = re.compile("(?<=<foaf:made rdf:resource=.).*(?=./>)")
				name = namePattern.findall(s)
				location = locationPattern.findall(s)
				homepage = homepagePattern.findall(s)
				photo = photoPattern.findall(s)
				recordList = recordListPattern.findall(s)
				self._tempRecord('temp_16',name)
				self._tempRecord('temp_17',location)
				self._tempRecord('temp_18',homepage)
				self._tempRecord('temp_19',photo)
				self._tempRecord('temp_20',recordList)
				print'Extracting \'artist name\':..........  ..........'
				print name
				print'Extracting \'artist location\':..........  ..........'
				print location
				print'Extracting \'artist homepage\':..........  ..........'
				print homepage
				print'Extracting \'artist photo url\':..........  ..........'
				print photo
				print'Extracting \'artist\'s work list \':..........  ..........'
				print recordList


	def spider(self):
		ac = AudioIDLink()
		#editoralData = []
		(zitgisturi, trackUrl3, MBID) = ac.metadata(self.filename)
		trackUrl3=trackUrl3+'.html'
		print 'Getting web identification \'MBID\'..........',MBID
		startURL = sparqlFront+zitgisturi+sparqlEnd
		(trackUrl,trackUrl2)=self._getTrackURL(startURL)
		(recordUrl,recordUrl2)=self._getRecordURL(trackUrl,trackUrl2,trackUrl3)
		artistUrl=self._getArtistURL(recordUrl,recordUrl2)
		self._getArtistData(artistUrl)
		return None
		#testtest=('aa','bb','cc')
		#return testtest
		#url = 'http://dbtune.org/jamendo/sparql/?query=describe%20%3Chttp://zitgist.com/music/track/c7190fdc-72de-41b0-aa64-9fdc3bb7380e%3E'
