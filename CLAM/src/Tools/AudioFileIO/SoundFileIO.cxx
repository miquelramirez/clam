#include "SoundFileIO.hxx"
#include "ErrSoundFileIO.hxx"

using namespace CLAM;

SoundFileIO::SoundFileIO()
{
	mMode = eNone;
	Init();
}

SoundFileIO::~SoundFileIO()
{
	Close();
}

void SoundFileIO::Init(void)
{
	mFile = 0;
	mPos = 0;
	mOffset = 0;
	mSize = 0;
	mSwap = false;
	mStdIO = false;
	
	InitSelf();
}

void SoundFileIO::Open(const char* filename,EMode mode)
{
	char* cmode = 0;

	Init();
	
	mMode = mode;

	if (filename[0]=='-' && filename[1]=='\0')
	{
		switch (mMode)
		{
			case eRead:
				mFile = stdin; break;
			case eWrite:
				mFile = stdout; break;
			default:
				throw ErrSoundFileIO("Invalid mode to open stdio");
		}
		mStdIO = true;
	}
	else
	{
		switch (mMode)
		{
			case eRead:
				cmode = "rb"; break;
			case eWrite:
				cmode = "wb"; break;
			case eDuplex:
				cmode = "rb+"; break;
			default:
				throw ErrSoundFileIO("Invalid mode to open soundfile");
		}
	
		mFile = fopen(filename,cmode);

		if (mFile==0) 
		  throw ErrSoundFileIO("File not found");
	}

	ReadHeader();
	
	Seek(0);
}

void SoundFileIO::Create(const char* filename,EMode mode,const SoundHeader& header)
{
	char* cmode = 0;

	mHeader = header;

	Init();

	mMode = mode;
	
	if (filename[0]=='-' && filename[1]=='\0')
	{
		switch (mMode)
		{
			case eRead:
				mFile = stdin; break;
			case eWrite:
				mFile = stdout; break;
			default:
				throw ErrSoundFileIO("Invalid mode to open stdio");
		}
		mStdIO = true;
	}
	else
	{
		switch (mMode)
		{
			case eRead:
				throw ErrSoundFileIO("Cannot create a file for reading");
			case eWrite:
				cmode = "wb"; break;
			case eDuplex:
				cmode = "wb+"; break;
			default:
				throw ErrSoundFileIO("Invalid mode to open soundfile");
		}
		mFile = fopen(filename,cmode);

		if (mFile==0) throw ErrSoundFileIO("Cannot create file");
	}
	WriteHeader();
}

void SoundFileIO::Close(void)
{
	if (mFile) {
		WriteHeader();
		fclose(mFile);
	}
	mFile = 0;
}

int SoundFileIO::Read(float *out,int& size)
{
	/** PCM encoding*/
	int n = 0;

	if ( mHeader.mFormatTag == 0x0001 )
	{
		if ( mHeader.mSampleWidth == 8 )
		{
			unsigned char* data = new unsigned char[size];
			n = int( fread(data,1,size,mFile) );
			mPos += n;
			if (n!=size) 
				size = n;
			//throw ErrSoundFileIO("Could not read requested size");

			for ( int j = 0; j < size; j++ )
			  {
			    // MRJ: According to some dox found in http://www.wotsit.org
			    // 8-bit PCM WAV samples are always unsigned!
				out[j] = float( short(data[j])-128)/128.0f;
			  }

			delete [] data;
		}
		else if (mHeader.mSampleWidth == 16 )
		{	
			short* data = new short[size];

			n = int( fread(data,2,size,mFile) );
			mPos += n;
			if (n!=size) 
				size = n;
				//throw ErrSoundFileIO("Could not read requested size");

			if (mSwap)
				for (int i=0;i<size;i++) Swap(data[i]);

			for ( int j = 0; j < size; j++ )
			{
				out[j] = data[j] / 32768.f;
			}

			delete [] data;
		}
		else if ( mHeader.mSampleWidth == 24 )
		{
			int* data = new int[size];

			n = int( fread(data,3,size,mFile) );
			mPos += n;
			if (n!=size)
				size = n;
				//throw ErrSoundFileIO("Could not read requested size");

			char* cdata = (char*) data;
			if (mSwap)
			{
				for (int i=size-1;i>=0;i--)
					data[i] = 
						(cdata[i*3+1]<<8)+
						(cdata[i*3]<<16)+
						(cdata[i*3+2]<<8);
			} else {
				for (int i=size-1;i>=0;i--)
					data[i] = 
						(cdata[i*3+1]<<16)+
						(cdata[i*3]<<8)+
						(cdata[i*3+2]);
			}

			for ( int j = 0; j < size; j++ )
			{
				out[j] = data[j] / 8388608.f;
			}

			delete [] data;

		}
		else if ( mHeader.mSampleWidth == 32 )
		{
			int* data = new int[size];

			n = int( fread(data,4,size,mFile) );
		
			mPos += n;
			
			if (n!=size)
				size = n;
				//throw ErrSoundFileIO("Could not read requested size");

			if (mSwap)
				for (int i=0;i<size;i++) Swap(data[i]);

			for ( int j = 0; j < size; j++ )
			{
				out[j] = data[j] / 2147483648.f;
			}

			delete [] data;

		}
		else 
		{
			throw ErrSoundFileIO( "Unsupported PCM sample format: supported values are"
								  "8, 16, 24 and 32 bits per sample");
		}
	}
	else if ( mHeader.mFormatTag == 0x0003 ) // IEEE float
	{
		float* data = new float[size];

		n = int( fread(data,4,size,mFile) );
	
		mPos += n;
		
		if (n!=size)
			size = n;
			//throw ErrSoundFileIO("Could not read requested size");

		if (mSwap)
			for (int i=0;i<size;i++) Swap(data[i]);

		for ( int j = 0; j < size; j++ )
		{
			out[j] = data[j];
		}

		delete [] data;

	}
	else 
	{
		throw ErrSoundFileIO( "Unsupported encoding: only PCM and IEEE float encodings");
	}

	return n;
}

int SoundFileIO::Write(const short *data,int size)
{
	if (mHeader.mSampleWidth !=16)
		throw ErrSoundFileIO("Samplewidth doesn't match");

	int n = 0;
	if (mSwap)
	{
		short tmp[256];
		while (n!=size) {
			int m = size-n;
			if (m>256) m=256;
			for (int i=0;i<m;i++) {
				tmp[i]=data[i]; 
				Swap(tmp[i]);
			}
			m = int( fwrite(tmp,2,m,mFile) );
			if (m<-1) 
				throw ErrSoundFileIO("Could not write requested size");
			n += m;
		}
	}else{
		n = int( fwrite(data,2,size,mFile) );
	}
	mPos += n;
	if (mPos>mSize) mSize = mPos;
	if (n!=size) throw ErrSoundFileIO("Could not write requested size");
	return n;
}

int SoundFileIO::Read(int *data,int size)
{
	int n = int( fread(data,mHeader.mBytesPerSample,size,mFile) );
	mPos += n;
	if (n!=size) throw ErrSoundFileIO("Could not read requested size");

	char* cdata = (char*) data;

	if (mHeader.mBytesPerSample==3)
	{
		printf("%d",mSwap);
		if (mSwap)
		{
			for (int i=size-1;i>=0;i--)
				data[i] = 
					(cdata[i*3+1]<<8)+
					(cdata[i*3]<<16)+
					(cdata[i*3+2]<<8);
		} else {
			for (int i=size-1;i>=0;i--)
				data[i] = 
					(cdata[i*3+1]<<16)+
					(cdata[i*3]<<8)+
					(cdata[i*3+2]);
		}
	}else if (mHeader.mBytesPerSample==4)
	{
			if (mSwap)
				for (int i=0;i<size;i++) Swap(data[i]);
	}else{
		throw ErrSoundFileIO("Incorrect samplewidth");
	}
	
	return n;
}

int SoundFileIO::Write(const int *data,int size)
{
	throw ErrSoundFileIO("NOT IMPLEMENTED");
	return 0;
/*
	int n = 0;
	if (mSwap)
	{
		short tmp[256];
		while (n!=size) {
			int m = size-n;
			if (m>256) m=256;
			for (int i=0;i<m;i++) {
				tmp[i]=data[i]; 
				Swap(tmp[i]);
			}
			m = fwrite(tmp,mHeader.mBytesPerSample,m,mFile);
			if (m<-1) 
				throw ErrSoundFileIO("Could not write requested size");
			n += m;
		}
	}else{
		n = fwrite(data,2,size,mFile);
	}
	mPos += n;
	if (mPos>mSize) mSize = mPos;
	if (n!=size) throw ErrSoundFileIO("Could not write requested size");
	return n;
*/
}

void SoundFileIO::Seek(int pos)
{
	mPos = pos;

	if (mPos<0 || mPos>mSize)
		throw ErrSoundFileIO("Position out of range");
	
	if (fseek(mFile,mOffset+mPos*mHeader.mBytesPerSample,SEEK_SET))
		throw ErrSoundFileIO("Could not set requested file position");
}

void SoundFileIO::SeekFrame(int frame)
{
	Seek(frame*mHeader.mChannels);
}

int SoundFileIO::Tell(void)
{
	return mPos;
}

int SoundFileIO::TellFrame(void)
{
	return mPos/mHeader.mChannels;
}
