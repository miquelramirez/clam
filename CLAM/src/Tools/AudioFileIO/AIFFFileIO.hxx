#ifndef __AIFFFileIO__
#define __AIFFFileIO__

#include "SoundFileIO.hxx"

class AIFFFileIO:public SoundFileIO
{
public:
	~AIFFFileIO()
	{
		Close();
	}
private:
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
		unsigned int len;
	};
	struct extended{
		unsigned char bytes[10];
	};

#pragma pack (2)

	struct AIFFCommChunk 
	{
		short			channels;
		unsigned int numSampleFrames;
		short			sampleWidth;
		extended		samplerate;
	};

#pragma pack ()

	int ReadChunkHeader(ChunkHeader& h);
	int ReadID(ID& id);

	int WriteChunkHeader(const ChunkHeader& h);
	int WriteID(ID& id);

	bool CheckID(const ID& id,const ID& cmp);

	void InitSelf(void);
};

#endif
