/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __NETWORKPRESENTATION_HXX__
#define __NETWORKPRESENTATION_HXX__

#include <CLAM/Presentation.hxx>
#include <CLAM/Slotv1.hxx>
#include <CLAM/Slotv0.hxx>
#include <CLAM/Slotv2.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Signalv2.hxx>

#include <string>
#include <list>

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{
	class NetworkController;
	class ProcessingController;
//	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ProcessingPresentation;
class ConnectionPresentation;
class ConnectionPointPresentation;

class NetworkPresentation: public CLAMVM::Presentation
{
	CLAMVM::NetworkController * mNetworkController;

protected:
	std::string mName;
	std::list<ProcessingPresentation*> mProcessingPresentations;
	std::list<ProcessingPresentation*> mProcessingPresentationsToRemove;
	typedef std::list<ProcessingPresentation*>::iterator ProcessingPresentationIterator;
	typedef std::list<ProcessingPresentation*>::reverse_iterator ProcessingPresentationReverseIterator;
	std::list<ConnectionPresentation*> mConnectionPresentations;
	std::list<ConnectionPresentation*> mConnectionPresentationsToRemove;
	typedef std::list<ConnectionPresentation*>::iterator ConnectionPresentationIterator;

public:
	NetworkPresentation();
        virtual ~NetworkPresentation();
	virtual void AttachToNetworkController(CLAMVM::NetworkController &);
	virtual void Show() = 0;
	virtual void Hide() = 0;
	void UpdatePresentations();
	void Clear( );
	virtual void SetUpWidgetsPositions(const std::string& positionsFilename ) = 0;
	virtual void SaveWidgetsPositions(const std::string& positionsFilename ) = 0;

	CLAMVM::NetworkController & GetNetworkController();

	virtual void CreatePortConnectionPresentation( const std::string &, const std::string & ) = 0;
	virtual void CreateControlConnectionPresentation( const std::string &, const std::string & ) = 0;
	void RemoveConnectionPresentation( const std::string &, const std::string & );
	void RebuildProcessingPresentationAttachedTo( const std::string &, CLAMVM::ProcessingController * );
	virtual void CreateProcessingPresentation( const std::string &, CLAMVM::ProcessingController * ) = 0;
	void ChangeConnectionPresentationNames( const std::string &, const std::string & );
protected:
	virtual void SetName(const std::string& name) = 0; 
	virtual void StartThread();
	virtual void StopThread();
	virtual void ChangeOSCState( bool );

	void RemoveProcessing( ProcessingPresentation* );
	void AddProcessing2Remove( const std::string & , CLAM::Processing * );
	std::string AddProcessing( const std::string& key );
	
	void RemovePortConnection( ConnectionPresentation * );
	void RemoveControlConnection( ConnectionPresentation * );
	void CreatePortConnection( const std::string &, const std::string & );
	void CreateControlConnection( const std::string &, const std::string & );


	ConnectionPointPresentation & GetOutPortPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetInPortPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetOutControlPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetInControlPresentationByCompleteName(const std::string &);

	/** TODO all this methods are repeated from the Network: To Refactor! */
	ProcessingPresentation& GetProcessingPresentation( const std::string & );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();
	static std::size_t PositionOfLastIdentifier( const std::string & );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );

public: 
	SigSlot::Slotv0 SlotStartThread;
	SigSlot::Slotv0 SlotStopThread;
	SigSlot::Slotv1< bool > SlotChangeOSCState;
//	SigSlot::Signalv1 SignalStartThread;
//	SigSlot::Signalv1 SignalStopThread;
//	SigSlot::Signalv1< const std::string & > SignalLoadNetworkFrom;
//	SigSlot::Signalv1< const std::string & > SignalSaveNetworkTo;
//	SigSlot::Signalv0 SignalClear;
	SigSlot::Slotv0 SlotClear;
	SigSlot::Slotv2< const std::string &, CLAM::Processing *  > SlotAddProcessing;
//	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > SignalAddProcessing;
//	SigSlot::Slotv2< const std::string & , CLAMVM::ProcessingController * > SlotCreateProcessingPresentation;
	SigSlot::Slotv1< ProcessingPresentation* > SlotRemoveProcessing;
//	SigSlot::Signalv1< const std::string & > SignalRemoveProcessing;
//	SigSlot::Slotv2< const std::string &, CLAMVM::ProcessingController * > SlotRebuildProcessingPresentationAttachedTo;
//	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreatePortConnection;
//	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreatePortConnection;
//	SigSlot::Signalv2< const std::string &, const std::string &  > SignalRemovePortConnection;
	SigSlot::Slotv1< ConnectionPresentation * > SlotRemovePortConnection;
//	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreatePortConnectionPresentation;	
//	SigSlot::Slotv2< const std::string &, const std::string &> SlotRemoveConnectionPresentation;
//	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreateControlConnection;
//	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreateControlConnection;
//	SigSlot::Signalv2< const std::string &, const std::string & > SignalRemoveControlConnection;
	SigSlot::Slotv1< ConnectionPresentation *> SlotRemoveControlConnection;
//	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreateControlConnectionPresentation;
//	SigSlot::Slotv2< const std::string &, const std::string & > SlotChangeConnectionPresentationNames;
};

} // namespace NetworkGUI

#endif // __NETWORKPRESENTATION_HXX__
