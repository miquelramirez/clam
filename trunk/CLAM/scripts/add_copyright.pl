$nowyear = 2004;
@files = split "\n",`find ../src ../examples -name "*.[ch]xx"`;
foreach $file (@files)
{
	$contents = "";
	open INFILE,$file;
	while (<INFILE>)
	{
		$contents = $contents.$_;
	}
	close INFILE;
	if (!($contents=~ m/under[\s\*]+the[\s\*]+terms[\s\*]+of[\s\*]+the[\s\*]+GNU[\s\*]+General[\s\*]+Public[\s\*]+License/s))
	{
		print "$file: adding copyright notice\n";
		open OUTFILE,">$file";
		print OUTFILE<<EOF;
/*
 * Copyright (c) $nowyear MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

EOF
		$contents =~ s/^\s*//s;
		print OUTFILE $contents;
		close OUTFILE;
	}elsif ($contents =~ s/(Copyright \(c\) )(\d\d\d\d-)(\d\d\d\d)( MUSIC TECHNOLOGY GROUP)/$1$2$nowyear$4/s && $3!=$nowyear)
	{
		print "$file: moved copyright $2$3 to $2$nowyear\n";
		open OUTFILE,">$file";
		print OUTFILE $contents;
		close OUTFILE;
	}elsif ($contents =~ s/(Copyright \(c\) )(\d\d\d\d)( MUSIC TECHNOLOGY GROUP)/$1$2-$nowyear$3/s && $2!=$nowyear)
	{
		print "$file: moved copyright $2 to $2-$nowyear\n";
		open OUTFILE,">$file";
		print OUTFILE $contents;
		close OUTFILE;
	}
}
