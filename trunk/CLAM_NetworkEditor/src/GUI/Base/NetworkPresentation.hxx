
#ifndef __NETWORKPRESENTATION_HXX__
#define __NETWORKPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Slotv1.hxx"
#include "Slotv0.hxx"
#include "Slotv2.hxx"
#include "Signalv0.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

#include <string>
#include <list>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class NetworkModel;
	class ProcessingController;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ProcessingPresentation;
class ConnectionPresentation;
class OutPortPresentation;
class InPortPresentation;
class OutControlPresentation;
class InControlPresentation;

class NetworkPresentation: public CLAMVM::Presentation
{
protected:
	std::string mName;
	bool mNetworkState;
	std::list<ProcessingPresentation*> mProcessingPresentations;
	typedef std::list<ProcessingPresentation*>::iterator ProcessingPresentationIterator;
	std::list<ConnectionPresentation*> mConnectionPresentations;
	typedef std::list<ConnectionPresentation*>::iterator ConnectionPresentationIterator;

public:
	NetworkPresentation();
        virtual ~NetworkPresentation();
	virtual void AttachTo(CLAMVM::NetworkModel &);
	virtual void Show() = 0;
	virtual void Hide() = 0;

protected:
	virtual void OnNewName(const std::string& name) = 0; 
	virtual void OnNewChangeState( bool );
	virtual void OnNewProcessing(CLAMVM::ProcessingController*, const std::string & ) = 0;
	virtual void OnNewPortConnection(CLAMVM::ConnectionAdapter* ) = 0;
	virtual void OnNewControlConnection(CLAMVM::ConnectionAdapter* ) = 0;
	virtual void OnRemovePortConnection( ConnectionPresentation* );
	virtual void OnRemoveControlConnection( ConnectionPresentation* );
	virtual void OnRemoveProcessing( ProcessingPresentation* );
	virtual void OnAddNewProcessing( const std::string & , CLAM::Processing * );
	virtual void OnClear( );


	// methods related to locate processing
	OutPortPresentation & GetOutPortPresentationByCompleteName(const std::string &);
	InPortPresentation & GetInPortPresentationByCompleteName(const std::string &);
	OutControlPresentation & GetOutControlPresentationByCompleteName(const std::string &);
	InControlPresentation & GetInControlPresentationByCompleteName(const std::string &);

	ProcessingPresentation& GetProcessingPresentation( const std::string & );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();
	static std::size_t PositionOfLastIdentifier( const std::string & );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );

public: //slots
	SigSlot::Slotv1<const std::string& > SetName;
	SigSlot::Slotv2< CLAMVM::ProcessingController*, const std::string & > SetProcessing;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter* > SetPortConnection;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter* > SetControlConnection;
	SigSlot::Slotv1< bool > ChangeState;
	SigSlot::Slotv1< ConnectionPresentation* > SetRemovePortConnection;
	SigSlot::Slotv1< ConnectionPresentation* > SetRemoveControlConnection;
	SigSlot::Slotv1< ProcessingPresentation* > SetRemoveProcessing;
	SigSlot::Slotv0 Clear;
	
	//signals
	SigSlot::Signalv2< const std::string &, const std::string & > CreateNewPortConnectionFromGUI;
	SigSlot::Signalv2< const std::string &, const std::string & > CreateNewControlConnectionFromGUI;

	SigSlot::Signalv1< bool > SChangeState;
	SigSlot::Signalv2< const std::string &, const std::string & > RemovePortConnectionFromGUI;
	SigSlot::Signalv2< const std::string &, const std::string & > RemoveControlConnectionFromGUI;
	SigSlot::Signalv1< const std::string & > RemoveProcessingFromGUI;
	SigSlot::Signalv1< const std::string & > LoadNetworkFrom;
	SigSlot::Signalv1< const std::string & > SaveNetworkTo;
	SigSlot::Signalv0 ClearSignal;

	SigSlot::Slotv2< const std::string &, CLAM::Processing *  > AddNewProcessing;
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > AddProcessing;
};

} // namespace NetworkGUI

#endif // __NETWORKPRESENTATION_HXX__
