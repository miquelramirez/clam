#include "WaveFileIO.hxx"
#include "AIFFFileIO.hxx"
#include "SoundFileIOError.hxx"

#include <math.h>

# define M_PI		3.14159265358979323846

main(int argc,char* argv_[])
{ 
	char* argv[]={
		"",
		"/export/temp/TheGarden.wav",
		"/export/temp/TheGarden.aif",
		"foo.wav"
	};
	
/*
	short buf[256];
	try {
		{
			WaveFileIO file;
			file.Open("Lm_Am_a1.wav",SoundFileIO::eRead);
			file.Close();

			AIFFFileIO file2;
			file2.Open("bar.aiff",SoundFileIO::eRead);
			file2.Close();
		}
		{
		  WaveFileIO file;
			SoundHeader header(44100,1);
	
			file.Create("test.wav",SoundFileIO::eWrite,header);
			float g=0,f=0;
			for (int j=0;j<100;j++)
			{	
					for (int i=0;i<100;i++)
					{
						float v = 1;
						for (float z=1;z<4;z*=1.5)
							v*=sin(g*z);
						buf[i]=short(v*sin(f)*32000.);
						g+=0.05;
						f+=0.0001;
						if (g>2.*M_PI) g-=2.*M_PI;
					}
					file.Write(buf,100);
			}
			file.Close();
		}
	}
	catch (SoundFileIOError err)
	{
		printf(err.mStr);
	}
*/

	short buf16[256];
	int buf24[256];
	try {
		WaveFileIO infile,outfile;
		AIFFFileIO infile2;
		SoundHeader header(44100,2);

		infile.Open(argv[1],WaveFileIO::eRead);
		infile2.Open(argv[2],AIFFFileIO::eRead);
		outfile.Create(argv[3],WaveFileIO::eWrite,header);

		infile.SeekFrame(441000);
		infile2.SeekFrame(441000);

		int n;
		
		n = 88100; //infile.Size();		
		while (n)
		{
			int m = n;
			if (m>256) m=256;
			n-=m;
			
			infile.Read(buf24,m);
			for (int i=0;i<m;i++)
				buf16[i]=buf24[i]>>8;
			outfile.Write(buf16,m);
		}
		n = 88100; //infile.Size();		
		while (n)
		{
			int m = n;
			if (m>256) m=256;
			n-=m;
			
			infile2.Read(buf24,m);
			for (int i=0;i<m;i++)
				buf16[i]=buf24[i]>>8;
			outfile.Write(buf16,m);
		}
		
		outfile.Close();
		infile.Close();
	}
	catch (SoundFileIOError err)
	{
		printf(err.mStr);
	}
	
/*
	try {
		WaveFileIO wavefile;
		SoundHeader header(22050,1);

		wavefile.Create(argv[1],WaveFileIO::eWrite,header);
		buf[0]=-1;
		wavefile.Write(buf,1);
		wavefile.Close();
		
		wavefile.Open(argv[1],WaveFileIO::eDuplex);
		wavefile.Seek(1);
		buf[0]=0;
		wavefile.Write(buf,1);
		buf[0]=1;
		wavefile.Write(buf,1);
		wavefile.Close();

		wavefile.Open(argv[1],WaveFileIO::eDuplex);
		wavefile.Seek(wavefile.Size());
		for (int j=0;j<100;j++)
		{
			for (int i=0;i<80;i++)
			{
				buf[i]=i*i;
			}
			wavefile.Write(buf,50);
		}

		wavefile.Close();

		short buf1[256];
		short buf2[256];
		short buf3[256];
		int i,j;
		wavefile.Open(argv[1],WaveFileIO::eDuplex);
		i = 0;
		j = wavefile.Size();
		int n = (j-i)/2;
		while (n>0) {
			if (n>256) n=256;
			wavefile.Seek(i);
			wavefile.Read(buf1,n);
			j -= n;
			wavefile.Seek(j);
			wavefile.Read(buf2,n);

			for (int k=0;k<n;k++)
				buf3[k]=buf1[n-k-1];
			wavefile.Seek(j);
			wavefile.Write(buf3,n);
			for (int k=0;k<n;k++)
				buf3[k]=buf2[n-k-1];
			wavefile.Seek(i);
			wavefile.Write(buf3,n);
			
			i += n;
			n = (j-i)/2;
		}
		wavefile.Close();
	}
	catch (SoundFileIOError err)
	{
		printf(err.mStr);
	}
*/
}
