#ifndef ClockBasedNetworkPlayer_hxx
#define ClockBasedNetworkPlayer_hxx

#include "NetworkPlayer.hxx"

namespace CLAM
{

class ClockBasedNetworkPlayer : public NetworkPlayer
{
public:
	ClockBasedNetworkPlayer()
	{
		std::cout << "helloworld" << std::endl;
	}

	virtual bool IsRealTime() const { return true; }
};

} // namespace CLAM
#endif

