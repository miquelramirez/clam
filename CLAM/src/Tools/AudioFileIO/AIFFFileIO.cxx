#include "extended.hxx"
#include "AIFFFileIO.hxx"
#include "ErrSoundFileIO.hxx"


using namespace CLAM;

void AIFFFileIO::InitSelf(void)
{
#ifdef SOUNDFILEIO_LITTLE_ENDIAN
	mSwap = true;
#endif
}

#ifdef SOUNDFILEIO_LITTLE_ENDIAN
#define SWAP(var) Swap(var)
#else
#define SWAP(var)
#endif

int AIFFFileIO::ReadChunkHeader(ChunkHeader& h)
{
	int ret = int( fread(&h,1,sizeof(h),mFile) );
	SWAP(h.len);
	return ret;
}

int AIFFFileIO::WriteChunkHeader(const ChunkHeader& h)
{
	ChunkHeader cp = h;
	SWAP(cp.len);
	return int(fwrite(&cp,1,sizeof(cp),mFile) );
}

int AIFFFileIO::ReadID(ID& id)
{
	return int( fread(&id,1,sizeof(id),mFile) );
}

int AIFFFileIO::WriteID(ID& id)
{
	return int( fwrite(&id,1,sizeof(id),mFile) );
}

bool AIFFFileIO::CheckID(const ID& id,const ID& cmp)
{
	return !memcmp(&id,&cmp,4);
}

void AIFFFileIO::ReadHeader(void)
{
	unsigned int i = 0;
	bool fmtFound = false;
	fseek(mFile,0,SEEK_SET);
	ID AIFFID;
	ChunkHeader form;
	ReadChunkHeader(form);
	if (!CheckID(form.id,"FORM"))
		throw ErrSoundFileIO("Not an AIFF file");
	i = 0;
	i += ReadID(AIFFID);
	if (!CheckID(AIFFID,"AIFF"))
		throw ErrSoundFileIO("Not an AIFF file");

	while (i<form.len) {
		ChunkHeader h;
		i += ReadChunkHeader(h);
		if (CheckID(h.id,"COMM")) {
			AIFFCommChunk fmt;
			i += int( fread(&fmt,1,sizeof(fmt),mFile) );

			if (sizeof(fmt)!=2+4+2+10) 
				throw ErrSoundFileIO("Incorrect size of AIFFCommChunk: check alignment");

			SWAP(fmt.channels);
			SWAP(fmt.numSampleFrames);
			SWAP(fmt.sampleWidth);

			mHeader.mSampleWidth = fmt.sampleWidth;
			mHeader.mBytesPerSample = (mHeader.mSampleWidth+7)>>3;
			mHeader.mChannels = fmt.channels;
			mHeader.mSamplerate = tenbytefloat2int(fmt.samplerate.bytes);
			mSize = fmt.numSampleFrames;
			fmtFound = true;
		}
		else{
			if (CheckID(h.id,"SSND")) {
				unsigned int offset;
				unsigned int blockSize;

				fread(&offset,1,sizeof(offset),mFile);
				fread(&blockSize,1,sizeof(blockSize),mFile);

				mOffset = sizeof(form)+i;
			}
			i += h.len;
			fseek(mFile, sizeof(form)+i, SEEK_SET);
		}
	}
	if (!fmtFound)
		throw ErrSoundFileIO("No format chunk found");
	if (mOffset == 0) 
		throw ErrSoundFileIO("No data block found");
}

void AIFFFileIO::WriteHeader(void)
{
	fseek(mFile,0,SEEK_SET);
	mOffset = 0;
	ChunkHeader formHeader("FORM");
	ChunkHeader fmtHeader("COMM",sizeof(AIFFCommChunk));
	ChunkHeader dataHeader("SSND",mSize*2);

	AIFFCommChunk fmtChunk;
	ID AIFFID("AIFF");

	if (mStdIO && mSize==0)
	{	
		dataHeader.len = 0x7FFFFFFF;
	}

	formHeader.len = 
		sizeof(AIFFID) + 
		sizeof(fmtHeader) + fmtHeader.len + 
		sizeof(dataHeader) + dataHeader.len;

	fmtChunk.channels = mHeader.mChannels;
	uint2tenbytefloat(mHeader.mSamplerate,fmtChunk.samplerate.bytes);
	fmtChunk.numSampleFrames = mSize;
	fmtChunk.sampleWidth = 16;
	
	mOffset += WriteChunkHeader(formHeader);
	mOffset += WriteID(AIFFID);
	mOffset += WriteChunkHeader(fmtHeader);

	SWAP(fmtChunk.channels);
	SWAP(fmtChunk.numSampleFrames);
	SWAP(fmtChunk.sampleWidth);
	
	mOffset += int( fwrite(&fmtChunk,1,sizeof(fmtChunk),mFile) );
	mOffset += WriteChunkHeader(dataHeader);

	unsigned int offset = 0;
	unsigned int blockSize = 0;

	SWAP(offset);
	SWAP(blockSize);

	mOffset += int( fwrite(&offset,1,sizeof(offset),mFile) );
	mOffset += int( fwrite(&blockSize,1,sizeof(blockSize),mFile) );
}


