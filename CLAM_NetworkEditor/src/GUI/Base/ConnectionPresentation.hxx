
#ifndef __CONNECTIONPRESENTATION_HXX__
#define __CONNECTIONPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv2.hxx"
#include "Signalv1.hxx"
#include <string>

namespace CLAM
{
	class Processing;
}

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

/*	CLAM::Processing * GetInProcessing()
	{
		return mInProc;
	}
	CLAM::Processing * GetOutProcessing()
	{
		return mInProc;
	}
*/

protected:
	virtual void OnNewNames( const std::string &, const std::string &);

//	CLAM::Processing * mOutProc;
//	CLAM::Processing * mInProc;
	std::string mOutName;
	std::string mInName;
	
public: //slots
	SigSlot::Slotv2< const std::string &, const std::string &> SetNames;
	SigSlot::Signalv1< ConnectionPresentation* > RemoveConnection;

};

} // namespace NetworkGUI

#endif // __CONNECTIONPRESENTATION_HXX__
