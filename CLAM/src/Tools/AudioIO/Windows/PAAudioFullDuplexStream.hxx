#ifndef __PAAUDIOFULLDUPLEXSTREAM__
#define __PAAUDIOFULLDUPLEXSTREAM__

#include "PAAudioStream.hxx"

namespace CLAM
{

class PAAudioFullDuplexStream : public PAAudioStream
{
public:
	PAAudioFullDuplexStream( PAAudioStreamConfig& cfg )
		: PAAudioStream( cfg )
	{
	}
protected:
	void SetupStream();
private:
	void CheckConsistency() throw (ErrPortAudio);
};

}

#endif // PAAudioFullDuplexStream.hxx
