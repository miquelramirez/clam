# Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
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


open LAT,"latency.c";
open HXX,">SndPcm.hxx";
open CXX,">SndPcm.cxx";

@functions =
 (setparams_stream,setparams_bufsize,setparams_set,setparams,readbuf,writebuf);

print CXX<<EOF;
/*
DON'T EDIT THIS FILE BY HAND, IT IS GENERATED 
BY THE SCRIPT SndPcmGen.pl FROM
alsa-lib/test/latency.c
SndPcm.cxx.in.head
SndPcm.cxx.in.tail
SndPcm.hxx.in.head
SndPcm.hxx.in.tail
*/

EOF

print HXX<<EOF;
/*
DON'T EDIT THIS FILE BY HAND, IT IS GENERATED 
BY THE SCRIPT SndPcmGen.pl FROM
alsa-lib/test/latency.c
SndPcm.cxx.in.head
SndPcm.cxx.in.tail
SndPcm.hxx.in.head
SndPcm.hxx.in.tail
*/

EOF

open IN,"SndPcm.cxx.in.head";
while (<IN>)
{
	print CXX;
}
close IN;

open IN,"SndPcm.hxx.in.head";
while (<IN>)
{
	print HXX;
}
close IN;


print CXX<<EOF;
/*
 * The functions which follow are taken from the latency test included
 * in the ALSA source distribution, with the following copyright note:
 *
EOF
print HXX<<EOF;
/*
 * The functions which follow are taken from the latency test included
 * in the ALSA source distribution, with the following copyright note:
 *
EOF

while (<LAT>)
{
	next if (m%/\*%);
	print CXX $_;
	print HXX $_;
	last if (m%\*/%);
}

while (<LAT>)
{
	foreach $f (@functions)
	{
		if (s/^(\w+ +)$f/$1SndPcm::$f/)
		{
			$infunction = $f;
			$inproto = $f;
		}
	}
	if ($infunction)
	{
		s/, size_t \*frames//;
		s/, size_t \*max//;
		s/^[ |\t]+\*frames.*//;
		s/^[ |\t]+snd_pcm_dump.*//;
		s/.*\*max.*//;
		s/exit\(.*\)/return -1;/;
		s/printf/cat_error/;
		print CXX $_;
	}
	if (m/^{/ && $inproto)
	{
		$inproto = 0;
	}
	if ($inproto)
	{
		s/SndPcm:://;
		s/\)/\);/;
		print HXX "\t".$_;
	}
	if (m/^}/ && $infunction)
	{
		$infunction = 0;
		print CXX "\n";
	}
}

open IN,"SndPcm.cxx.in.tail";
while (<IN>)
{
	print CXX;
}
close IN;

open IN,"SndPcm.hxx.in.tail";
while (<IN>)
{
	print HXX;
}
close IN;

close HXX;
close CXX;
