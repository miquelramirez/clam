#ifndef __PAAUDIOOUTPUTSTREAM__
#define __PAAUDIOOUTPUTSTREAM__

#include "PAAudioStream.hxx"

namespace CLAM
{

class PAAudioOutputStream : public PAAudioStream
{
public:
	PAAudioOutputStream( PAAudioStreamConfig& cfg )
		: PAAudioStream( cfg )
	{
	}

protected:
	void SetupStream();
private:
	void CheckConsistency() throw (ErrPortAudio);
};

}

#endif // PAAudioOutputStream.hxx
