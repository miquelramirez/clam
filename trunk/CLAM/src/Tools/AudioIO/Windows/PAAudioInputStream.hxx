#ifndef __PAAUDIOINPUTSTREAM__
#define __PAAUDIOINPUTSTREAM__

#include "PAAudioStream.hxx"

namespace CLAM
{

class PAAudioInputStream : public PAAudioStream
{
public:
	PAAudioInputStream( PAAudioStreamConfig& cfg )
		: PAAudioStream( cfg )
	{
	}
protected:
	void SetupStream();
private:
	void CheckConsistency() throw (ErrPortAudio);
};

}

#endif // PAAudioInputStream.hxx
