
#ifndef __CONNECTIONPRESENTATION_HXX__
#define __CONNECTIONPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv2.hxx"
#include "Signalv3.hxx"
#include <string>


namespace CLAMVM
{
	class ConnectionModel;
}

namespace NetworkGUI
{

class ConnectionPresentation : public CLAMVM::Presentation
{
public:
	ConnectionPresentation();
	virtual ~ConnectionPresentation();
	virtual void AttachTo(CLAMVM::ConnectionModel & );
	virtual void Show() = 0;
	virtual void Hide() = 0;

	const std::string &GetInName()
	{
		return mInName;
	}
	const std::string & GetOutName()
	{
		return mOutName;
	}

protected:
	virtual void OnNewNames(const std::string &, const std::string &);
	std::string mInName;
	std::string mOutName;
	
public: //slots
	SigSlot::Slotv2< const std::string &, const std::string & > SetNames;
	SigSlot::Signalv3< const std::string &, const std::string &, ConnectionPresentation* > RemoveConnection;

};

} // namespace NetworkGUI

#endif // __CONNECTIONPRESENTATION_HXX__
