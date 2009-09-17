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

};

static void foo()
{
	static ClockBasedNetworkPlayer foo;
}
} // namespace CLAM
#endif

