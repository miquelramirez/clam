$appdir = $ARGV[0];
$appdir =~ s/\/$//;
if (!($appdir =~ m/\.app$/))
{
	die "first argument should be app dir";
}
$exec = $appdir;
$exec =~ s/\.app$//;
@files = ("$appdir/Contents/MacOS/$exec");
for ($i=0;$i<=$#files;$i++)
{
	print ">> $file\n";
	$file = $files[$i];
	$tmp = `otool -L $file`;
	while ($tmp =~ m/^\s+(\/Users.*.dylib)/gm)
	{
		$liborig = $1;
		$liborig =~ m/.*\/(.*)/;
		$libbase = $1;
		print "$liborig $libbase\n";
		if (!$copied{"$libbase"})
		{
			system "mkdir -p $appdir/Contents/Frameworks/";
			system "cp $liborig $appdir/Contents/Frameworks/";
			if (!$infiles{"$appdir/Contents/Frameworks/$libbase"})
			{
				push(@files,"$appdir/Contents/Frameworks/$libbase");
				$infiles{"$appdir/Contents/Frameworks/$libbase"} = 1;
			}
			$copied{"$libbase"} = 1;
		}
		system "install_name_tool -change $liborig \@executable_path/../Frameworks/$libbase $file";
	}
}
