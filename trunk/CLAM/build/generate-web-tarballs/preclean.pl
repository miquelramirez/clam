if ($#ARGV==-1)
{
	print "Error: Missing argument. Usage: DIR [limit (in KB)\n";
	exit(-1);
}

$limit = $ARGV[1]*1024;

sub recurse
{
	my $path = shift;
	my $d;
	
	if (-e "$path/.not-public")
	{
		print "removing $path (.not-public)\n";
		system "rm","-rf",$path;
		return;
	}
	my $DIR;
	opendir($DIR, $path);
	while ($d = readdir($DIR))
	{
		if (-d "$path/$d")
		{
			if ($d ne '.' && $d ne '..')
			{
				if ($d eq "CVS")
				{
					print "Error: found CVS dir in $path\n";
					exit(-1);
				}else{
					recurse("$path/$d");
				}
			}
		}
		elsif (-f "$path/$d")
		{
			my ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
                      	    $atime,$mtime,$ctime,$blksize,$blocks)
				= stat("$path/$d");
			if ($limit && $size>$limit)
			{
				print "Removing file $path/$d ; size $size > $limit\n";
				system "rm","$path/$d";
			}
		}else{
			print "Error: $path/$d is not a regular file or dir\n";
			exit(-1);
		}
	}
	closedir($DIR);
}


recurse($ARGV[0]);

