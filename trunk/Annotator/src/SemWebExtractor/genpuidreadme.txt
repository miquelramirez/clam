=============================================================================
GenPUID v1.1
-----------------------------------------------------------------------------
Build Date: July 18, 2007
Release Date: July 19, 2007
Platforms: Win32, Linux, Mac OS X
-----------------------------------------------------------------------------
The MusicIP product team welcomes your comments! Please contact us by writing
musicdns-feedback@musicip.com.
=============================================================================

INSTALLATION
============

GenPUID is a utility distributed as part of MusicDNS. The installation is as
simple as placing it in the directory you want the binary to reside in.
Please note that both the mipcore and AACTagReader binaries need to be in the
same directory, as GenPUID requires both in order to execute properly.

NOTE: AACTagReader is not used on Linux since AAC files are not supported.

INSTRUCTIONS
============

From the command line, run:
genpuid [dns-key] [options] file1 file2 ...

This will identify the files, invoking fingerprints and analysis as
necessary, and print out the PUID retrieved from MusicDNS.

Your MusicDNS key (used by the parameter [dns-key]) must be valid. Keys can
be generated for free at: http://musicip.com/dns/key.jsp.

----------
I. OPTIONS
----------

[*] You can use -r to pass directories and recursively process the files
    in them.

[*] You can use -xml to generate the output in XML format.

[*] -logex can be used to create a log file in the directory from which
	GenPUID is being executed.

[*] You can use -m3lib=cache.m3lib to store the results in a MusicIP
    Mixer cache file. If the cache already exists, it will simply be
    updated.

[*] You can use -rmd (return metadata) to include metadata in the response.
 	This switch can be used as follows:

	-rmd=1 includes basic artist name and track title metadata
	-rmd=2 includes all metadata: release year, genre, and albums

[*] You can use -noanalysis if you want to retrieve existing PUIDs only,
    and do not wish to create new PUIDs.

[*] The switch -fs=[encoding type] allows you to indicate the encoding type
 	to use. Usually, this is ISO8859-1. 

	Please note that this switch is only supported on Linux, and the
	Linux distribution being used must support the given encoding type
	for it to work properly.

[*] You can use -archive to burn MusicIP tags into song files as they are
    identified.

-----------
II. RESULTS
-----------

[*] When asked to return metadata (using the -xml) switch, the following will
	be returned:

    <genpuid songs="1">
    	<track file="c:\path\file.mp3" puid="123-456" />
    </genpuid>

	If the -xml switch is not used, the result will appear as follows:
	
	c:\path\file.mp3
	puid: 123-456
	
	If a PUID does not exist for the given file, just the file name will be
	returned.

[*] If a track does not have a PUID, the status returned will be
	"Unavailable." The example below also takes into account that the -xml
	switch is enabled.

    <genpuid songs="1">
    	<track file="c:\path\file.mp3">unavailable</track>
    </genpuid>

	Please note that the "unavailable" status only applies when using the
	-noanalysis switch.

NOTE: Metadata and status information will not be returned in any format 
other than XML. More information on the structure that's used is available
on the MusicIP blog at:
http://blog.musicip.com/2006/12/18/musicdns-11-preview-more-free-data/

-----------
III. STATUS
-----------

If the requested metadata or associated PUID, the following status codes will
be returned accordingly:

[*] Pending 
	The server has not yet generated a PUID for this track, which can take up
	to 24 hours. If you have submitted a fingerprint for PUID creation, you 
	will need to re-request it from GenPUID after that time frame has lapsed.
	
[*] Unanalyzable
	GenPUID could not analyze the track, and this status may indicate an 
	encoding problem.

[*] Unknown
	Catch-all for other problems, such as the MusicDNS server being down.
	
[*] Unavailable
	This status indicates that the track provided to GenPUID does not yet
	have a PUID in the MusicIP database. To ensure the track is identifiable
	in the future, analysis can be executed.

Contact
=======

To post questions regarding GenPUID, please visit our forums at
http://forums.musicip.com/. You can also provide bug reports and enhancement
requests to the MusicIP product team by e-mailing 
musicdns-feedback@musicip.com.

© 2007 MusicIP Corporation. All rights reserved.
