#include "WaveFileIO.hxx"
#include "ErrSoundFileIO.hxx"

#ifdef SOUNDFILEIO_BIG_ENDIAN
#define SWAP(var) Swap(var)
#else
#define SWAP(var)
#endif

using namespace CLAM;

void WaveFileIO::InitSelf(void)
{
#ifdef SOUNDFILEIO_BIG_ENDIAN
	mSwap = true;
#endif
	mCuePoints = 0;
	mNCuePoints = 0;
}

int WaveFileIO::ReadChunkHeader(ChunkHeader& h)
{
	int ret = int( fread(&h,1,sizeof(h),mFile) );
	if (ret>0) {
		SWAP(h.len);
	}
	return ret;
}

int WaveFileIO::WriteChunkHeader(const ChunkHeader& h)
{
	ChunkHeader cp = h;
	SWAP(cp.len);
	return int( fwrite(&cp,1,sizeof(cp),mFile) );
}

int WaveFileIO::ReadID(ID& id)
{
	return int( fread(&id,1,sizeof(id),mFile) );
}

int WaveFileIO::WriteID(ID& id)
{
	return int( fwrite(&id,1,sizeof(id),mFile) );
}

bool WaveFileIO::CheckID(const ID& id,const ID& cmp)
{
	return !memcmp(&id,&cmp,4);
}

void WaveFileIO::ReadHeader(void)
{
	int i = 0;
	bool fmtFound = false;
	fseek(mFile,0,SEEK_SET);
	ID waveID;
	ChunkHeader riff;
	ReadChunkHeader(riff);
	if (!CheckID(riff.id,"RIFF"))
		throw ErrSoundFileIO("Not a RIFF file");
	i = 0;
	int r = ReadID(waveID);
	i += r;
	if (!CheckID(waveID,"WAVE"))
		throw ErrSoundFileIO("Not a WAVE file");

	ChunkHeader h;	
	
	// allowing to read beyond riff len
	while ((r = ReadChunkHeader(h)) > 0) {
		if (i>=riff.len) 
		{
			// WARNING: Reading beyond RIFF chunk. gnoise locates cue-points
			// here. Check with standatd if this is correct
		}
		i += r;
		if (CheckID(h.id,"fmt ")) {
			WaveFmtChunk fmt;
			int j = 0;
			j = int( fread(&fmt,1,sizeof(fmt),mFile) );
			i += j;

			SWAP(fmt.formatTag);
			SWAP(fmt.channels);
			SWAP(fmt.samplerate);
			SWAP(fmt.sampleWidth);

			mHeader.mSampleWidth = fmt.sampleWidth;
			mHeader.mBytesPerSample = (mHeader.mSampleWidth+7)>>3;

			if (fmt.formatTag!=0x0001)
				throw ErrSoundFileIO("Not a WAVE PCM file");
			mHeader.mChannels = fmt.channels;
			mHeader.mSamplerate = fmt.samplerate;
			fmtFound = true;

			while (j<h.len)
			{
				char dum[256];
				int n = (h.len-j);
				if (n>256) n = 256;
				j += int( fread(&dum,1,n,mFile) );
			}
		}
		else
		{
		  if (CheckID(h.id,"data")) {
			  mSize = h.len/2;
			  mOffset = sizeof(riff)+i;
		  }
		  if (CheckID(h.id,"cue "))
			{
				if (mCuePoints)
				{
					delete mCuePoints;
				}
				mNCuePoints = (h.len-4)/24;
				int tmp;
				fread(&tmp,1,sizeof(int),mFile);
				SWAP(tmp);
				if (tmp!=mNCuePoints)
					throw ErrSoundFileIO("'cue ' chunk len does not match with number of cue points");
				mCuePoints = new CuePoint[mNCuePoints];
				for (int i=0;i<mNCuePoints;i++)
				{
					fread(&mCuePoints[i],1,sizeof(CuePoint),mFile);
					SWAP(mCuePoints[i].identifier);
					SWAP(mCuePoints[i].position);
					SWAP(mCuePoints[i].chunkStart);
					SWAP(mCuePoints[i].blockStart);
					SWAP(mCuePoints[i].offset);
				}
		  }
			i += h.len;
			fseek(mFile, sizeof(riff)+i, SEEK_SET);
		}
	}
	if (!fmtFound)
		throw ErrSoundFileIO("No format chunk found");
	if (mOffset == 0) 
		throw ErrSoundFileIO("No data block found");
}

void WaveFileIO::WriteHeader(void)
{
	fseek(mFile,0,SEEK_SET);
	mOffset = 0;
	ChunkHeader riffHeader("RIFF");
	ChunkHeader fmtHeader("fmt ",sizeof(WaveFmtChunk));
	ChunkHeader dataHeader("data",mSize*2);

	if (mStdIO && mSize==0)
	{	
		dataHeader.len = 0x7FFFFFFF;
	}
	
	WaveFmtChunk fmtChunk;
	ID waveID("WAVE");

	riffHeader.len = 
		sizeof(waveID) + 
		sizeof(fmtHeader) + fmtHeader.len + 
		sizeof(dataHeader) + dataHeader.len;

	fmtChunk.formatTag = 0x0001;
	fmtChunk.channels = mHeader.mChannels;
	fmtChunk.samplerate = mHeader.mSamplerate;
	fmtChunk.bytesPerSec = 
		mHeader.mSamplerate*mHeader.mBytesPerSample*mHeader.mChannels;
	fmtChunk.blockAlign = mHeader.mBytesPerSample;
	fmtChunk.sampleWidth = mHeader.mSampleWidth;
	
	mOffset += WriteChunkHeader(riffHeader);
	mOffset += WriteID(waveID);
	mOffset += WriteChunkHeader(fmtHeader);

	SWAP(fmtChunk.formatTag);
	SWAP(fmtChunk.channels);
	SWAP(fmtChunk.samplerate);
	SWAP(fmtChunk.bytesPerSec);
	SWAP(fmtChunk.blockAlign);
	SWAP(fmtChunk.sampleWidth);
	
	mOffset += int( fwrite(&fmtChunk,1,sizeof(fmtChunk),mFile) );

	mOffset += WriteChunkHeader(dataHeader);
}

