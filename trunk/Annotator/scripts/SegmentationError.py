#!/usr/bin/python

# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


from Pool import *
import sys
from xml.dom.ext.reader import Sax2

def extractAttribute(pool, scope, attribute) :	
	return pool.SelectAttributes(scope,attribute)[0].firstChild.data.split()

def extractEnumAttribute(pool, scope, attribute) :	
	return [e.firstChild.data for e in pool.SelectAttributes(scope,attribute)[0].childNodes if e.nodeType == e.ELEMENT_NODE  ]

def segmentAtTime(segmentation, time) :	
	i = 0
	while i < len(segmentation) :
		if (time >= segmentation[i]) & (time < segmentation[i+1]) :
			return i/2
		i += 2

def chordAtTime(segmentation, chords, time) :
	if segmentAtTime(segmentation,time) != None :
		return chords[ segmentAtTime(segmentation,time) ]
	else :
		return 'None'

def numberOfChordTimeUnits(segmentation, hop, start, end) :
	result = 0
	time = start
	while time < end :
		if segmentAtTime(segmentation,time) != None :
			result += 1
		time += hop
	return result

def numberOfCorrectChordTimeUnits(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end) :
	result = 0
	time = start
	while time < end :
		if chordAtTime(computedSegmentation, computedChords, time) == chordAtTime(trueSegmentation, trueChords, time) :
			if chordAtTime(computedSegmentation, computedChords, time) != 'None' :
				result +=1
		time += hop
	return result

def calculateRecall(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start,end) :
	NumberCorrectChords = numberOfCorrectChordTimeUnits(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end)
	NumberTrueChords = numberOfChordTimeUnits(trueSegmentation, hop,start,end)
	print 'number of correctly computed chords:',NumberCorrectChords,'	number of true chords:',NumberTrueChords	
	recall = float(NumberCorrectChords)/float(NumberTrueChords)
	print 'RECALL: ',recall
	return recall

def calculatePrecision(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start,end) :
	NumberCorrectChords = numberOfCorrectChordTimeUnits(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end)
	NumberComputedChords = numberOfChordTimeUnits(computedSegmentation, hop,start,end)
	print 'number of correctly computed chords:',NumberCorrectChords,'	number of computed chords:',NumberComputedChords	
	precision = float(NumberCorrectChords)/float(NumberComputedChords)
	print 'PRECISION: ',precision
	return precision

def displayAllChords(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end) :
	print 'time:	segment_computed segment_true	chord_computed chord_true'
	time = start
	while time < end :
		time += hop
		print 'time:',time,'   segment:',segmentAtTime(computedSegmentation, time),segmentAtTime(trueSegmentation,time),'   chord:',chordAtTime(computedSegmentation,computedChords,time),chordAtTime(trueSegmentation,trueChords,time)





computedFile = '../example-data/SongsTest/Debaser-CoffeeSmell.mp3.chords'
trueFile = '../example-data/SongsTest/Debaser-WoodenHouse.mp3.chords'

computedPool = Pool(computedFile)
truePool = Pool(trueFile)

computedSegmentationString = extractAttribute(computedPool, "Song", "Chords_Harte")
computedSegmentation = [ float(a) for a in extractAttribute(computedPool, 'Song','Chords_Harte') ]
computedRoots = extractEnumAttribute(computedPool, "ExtractedChord", "Root")
computedModes = extractEnumAttribute(computedPool, "ExtractedChord", "Mode")
computedChords = []
for i in range(0,len(computedRoots)):
	computedChords.append(computedRoots[i]+' '+computedModes[i])

trueSegmentationString = extractAttribute(truePool, "Song", "Chords_Harte")
trueSegmentation = [ float(a) for a in extractAttribute(truePool, 'Song','Chords_Harte') ]
trueRoots = extractEnumAttribute(truePool, "ExtractedChord", "Root")
trueModes = extractEnumAttribute(truePool, "ExtractedChord", "Mode")
trueChords = []
for i in range(0,len(trueRoots)):
	trueChords.append(trueRoots[i]+' '+trueModes[i])

hop = 4096.0/44100.0
start = hop/2.0
end =  trueSegmentation[len(trueSegmentation)-1] # TODO: take number of frames from Pool

#displayAllChords(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end)

print 'hop:',hop
print 'start time:',start
print 'end time (end of ground truth\'s last segment)',end

calculateRecall(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end)
calculatePrecision(computedSegmentation, trueSegmentation, computedChords, trueChords, hop, start, end)
