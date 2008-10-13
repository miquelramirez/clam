#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

# Port of examples/FileInfo_example.cxx

from clam import *
import sys

if len(sys.argv)==1: # No input file
	print "No input file."
	print "Usage: file_info.py <input file name>"
	sys.exit(-1)
elif len(sys.argv)>2: #Too many parameters
	print "Too many parameters"
	print "Usage: file_info.py <input file name>"
	sys.exit(-1)
	
def error(msg):
	print msg
	return -1

def main(args):
	
	# Now we take argv[1] ( the second token in the command used to invoke this
	# program ) as the 'File Location'
	file = AudioFileSource()
	file.OpenExisting( sys.argv[1] )
	
	# Once set the location, the AudioFile object internally does several things:
	#   + It checks that the file exists and is readable ( it conforms to one of
	#     the supported formats )
	#   + Extracts useful header data, such as sampling rate, number of channels, etc.
	#   + Extracts some textual meta-data that could be embedded in the file itself

	# So first, let's check that the input file is readable
	if not file.IsReadable():
		print "Error: file", file.GetLocation(), "cannot be opened "
		print "or is encoded in an unrecognized format"
		sys.exit(-1)

	# If it was readable now we can safely assume we can access its header
	print "Filename:", toString( file.GetLocation() ) #FIXME: implicity convertible to string is not working
	
	# Now we print out the format it was encoded:
	#print "Format:", file.GetHeader().GetFormat() #FIXME: two calls in the same line only works from ipython console
	header = file.GetHeader()
	print header.GetFormat()
	
	# The number of channels in the file
	print "Channels:", file.GetHeader().GetChannels()
	
	# Its sample rate
	print "Sample Rate: ", file.GetHeader().GetSampleRate(), " Hz"

	# And its length. However this is not so straight-forward since we provide
	# file duration in *milliseconds*. This might not be the best way ( we know ) but
	# for now, CLAM sees times as milliseconds. We can try to convert it into something like
	# hh:mm:ss
	durationMs = file.GetHeader().GetLength()
	
	numberSeconds = durationMs / 1000.
	numberMinutes = (int)(numberSeconds / 60.)
	numberSeconds = numberSeconds - numberMinutes * 60.
	numberHours = int(numberMinutes / 60.)
	numberMinutes = numberMinutes - numberHours * 60.

	#// and now we print it formatted:
	print "Length (hh:mm:ss):", numberHours,":",numberMinutes,":",numberSeconds
	#print "Length (hh:mm:ss):", ("%f:%f:%f")%(round(numberHours,2),round(numberMinutes,2),round(numberSeconds,2))
	
	# Finally, we can also access textual meta-data, if it was present in the file, of course.
	# AudioFile's textual meta-data is stored in an AudioTextDescriptors object, that we
	# can access as follows:

	txtDesc = file.GetTextDescriptors()

	# For now, AudioTextDescriptors only can hold the following attributes:
	#  + Artist name
	#  + Title/Songname
	#  + Album/Movie/Show name
	#  + Track number ( in album )
	#  + Composer name
	#  + Performer name
	#
	#  Obviously, it will be strange that all (or any) of these attributes are available, so we must be
	# careful before accessing each attribute:
	if txtDesc.HasArtist():
		print "Artist name: ", toString( txtDesc.GetArtist() ) #FIXME
	if txtDesc.HasTitle():
		print "Title/Songname: ", toString( txtDesc.GetTitle() ) #FIXME
	if txtDesc.HasAlbum():
		print "Album/Movie/Show name: ", toString( txtDesc.GetAlbum() ) #FIXME
	if txtDesc.HasTrackNumber():
		print "Track number: ", toString( txtDesc.GetTrackNumber() ) #FIXME
	if txtDesc.HasComposer():
		print "Composer name: ", toString( txtDesc.GetComposer() ) #FIXME
	if txtDesc.HasPerformer():
		print "Performer name: ", toString( txtDesc.GetPerformer() ) #FIXME
	
	#// And that's all friends!

	return 0
#main()

if __name__ == '__main__':
	sys.exit( main(sys.argv) )
