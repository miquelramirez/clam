#ifndef __WaveFileIO__
#define __WaveFileIO__

#include "SoundFileIO.hxx"

namespace CLAM{

class WaveFileIO:public SoundFileIO
{
public:
	~WaveFileIO()
	{
		Close();
		if (mCuePoints)
			delete mCuePoints;
	}
private:
	void InitSelf(void);
	void ReadHeader(void);
	void WriteHeader(void);

	struct ID {
		ID(const char* str = "\0\0\0\0")
		{
			memcpy(id,str,4);
		}
		char id[4];
	};

	struct ChunkHeader {
		ChunkHeader()
		{
			len = 0;
		}
		ChunkHeader(const ID& _id,int _len = 0):id(_id),len(_len)
		{ 
		}
		ID  id;
		int len;
	};

	struct CuePoint {
	  int    identifier;
	  int    position;
	  ID      fccChunk;
	  int    chunkStart;
	  int    blockStart;
	  int    offset;
	};
	
#pragma pack (2)

	struct WaveFmtChunk 
	{
		short formatTag;
		short channels;
		int   samplerate;
		int   bytesPerSec;
		short blockAlign;
		short sampleWidth;
	};

#pragma pack ()

	int ReadChunkHeader(ChunkHeader& h);
	int ReadID(ID& id);

	int WriteChunkHeader(const ChunkHeader& h);
	int WriteID(ID& id);

	bool CheckID(const ID& id,const ID& cmp);

public:
	CuePoint* mCuePoints; // read only for now
	int mNCuePoints; 
};

};//CLAM

#endif
