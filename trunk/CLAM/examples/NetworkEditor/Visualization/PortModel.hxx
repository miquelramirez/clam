
#ifndef __PORTMODEL_HXX__
#define __PORTMODEL_HXX__

#include "Signalv1.hxx"
#include <string>

namespace CLAMVM
{
	class PortModel
	{
	public:
		PortModel(){}
		virtual ~PortModel(){}

		//signals
		SigSlot::Signalv1< const std::string &>  AcquireName;
//		SigSlot::Signalv1< const std::string &>  AcquireDataType;

	};
} // namespace CLAMVM

#endif // __INPORTMODEL_HXX__
