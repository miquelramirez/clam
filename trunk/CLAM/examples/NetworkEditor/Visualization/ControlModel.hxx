
#ifndef __CONTROLMODEL_HXX__
#define __CONTROLMODEL_HXX__

#include "Signalv1.hxx"
#include <string>

namespace CLAMVM
{
	class ControlModel
	{
	public:
		ControlModel(){}
		virtual ~ControlModel(){}

		//signals
		SigSlot::Signalv1< const std::string &>  AcquireName;
//		SigSlot::Signalv1< const std::string &>  AcquireDataType;

	};
} // namespace CLAMVM

#endif // __INCONTROLMODEL_HXX__
