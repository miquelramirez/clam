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
}

int WaveFileIO::ReadChunkHeader(ChunkHeader& h)
{
	int ret = fread(&h,1,sizeof(h),mFile);
	SWAP(h.len);
	return ret;
}

int WaveFileIO::WriteChunkHeader(const ChunkHeader& h)
{
	ChunkHeader cp = h;
	SWAP(cp.len);
	return fwrite(&cp,1,sizeof(cp),mFile);
}

int WaveFileIO::ReadID(ID& id)
{
	return fread(&id,1,sizeof(id),mFile);
}

int WaveFileIO::WriteID(ID& id)
{
	return fwrite(&id,1,sizeof(id),mFile);
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
	while (i<riff.len) {
		ChunkHeader h;
		i += ReadChunkHeader(h);
		if (CheckID(h.id,"fmt ")) {
			WaveFmtChunk fmt;
			i += fread(&fmt,1,sizeof(fmt),mFile);

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
		}
		else
		{
		  if (CheckID(h.id,"data")) {
			  mSize = h.len/2;
			  mOffset = sizeof(riff)+i;
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
	
	mOffset += fwrite(&fmtChunk,1,sizeof(fmtChunk),mFile);

	mOffset += WriteChunkHeader(dataHeader);
}

