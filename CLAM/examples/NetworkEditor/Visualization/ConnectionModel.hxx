
#ifndef __CONNECTIONMODEL_HXX__
#define __CONNECTIONMODEL_HXX__

#include "Signalv2.hxx"
#include <string>

namespace CLAMVM
{
	class ConnectionModel
	{
	public:
		ConnectionModel(){}
		virtual ~ConnectionModel(){}

		//signals
		SigSlot::Signalv2< const std::string &, const std::string &>  AcquireNames;
	};
} // namespace CLAMVM

#endif // __INCONNECTIONMODEL_HXX__
