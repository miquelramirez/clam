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
	class NetworkController;
	class ProcessingController;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class ProcessingPresentation;
class ConnectionPresentation;
class ConnectionPointPresentation;

class NetworkPresentation: public CLAMVM::Presentation
{
protected:
	std::string mName;
	bool mNetworkState;
	std::list<ProcessingPresentation*> mProcessingPresentations;
	std::list<ProcessingPresentation*> mProcessingPresentationsToRemove;
	typedef std::list<ProcessingPresentation*>::iterator ProcessingPresentationIterator;
	std::list<ConnectionPresentation*> mConnectionPresentations;
	std::list<ConnectionPresentation*> mConnectionPresentationsToRemove;
	typedef std::list<ConnectionPresentation*>::iterator ConnectionPresentationIterator;

public:
	NetworkPresentation();
        virtual ~NetworkPresentation();
	virtual void AttachTo(CLAMVM::NetworkController &);
	virtual void Show() = 0;
	virtual void Hide() = 0;
	void UpdatePresentations();

protected:
	virtual void SetName(const std::string& name) = 0; 
	virtual void ChangeState( bool );

	virtual void CreateProcessingPresentation( const std::string &, CLAMVM::ProcessingController * ) = 0;
	void RemoveProcessing( ProcessingPresentation* );
	void AddProcessing( const std::string & , CLAM::Processing * );

	virtual void CreatePortConnectionPresentation( CLAMVM::ConnectionAdapter * ) = 0;
	virtual void CreateControlConnectionPresentation( CLAMVM::ConnectionAdapter * ) = 0;
	void RemovePortConnection( ConnectionPresentation * );
	void RemoveControlConnection( ConnectionPresentation * );
	void CreatePortConnection( const std::string &, const std::string & );
	void CreateControlConnection( const std::string &, const std::string & );

	void RemoveConnectionPresentation( 	const std::string &, const std::string & );

	virtual void Clear( );
	
	ConnectionPointPresentation & GetOutPortPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetInPortPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetOutControlPresentationByCompleteName(const std::string &);
	ConnectionPointPresentation & GetInControlPresentationByCompleteName(const std::string &);

	ProcessingPresentation& GetProcessingPresentation( const std::string & );
	std::string GetProcessingIdentifier( const std::string& );
	static char NamesIdentifiersSeparator();
	static std::size_t PositionOfLastIdentifier( const std::string & );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );

public: 
	SigSlot::Slotv1< bool > SlotChangeState;
	SigSlot::Signalv1< bool > SignalChangeState;
	
	SigSlot::Signalv1< const std::string & > SignalLoadNetworkFrom;
	SigSlot::Signalv1< const std::string & > SignalSaveNetworkTo;

	SigSlot::Signalv0 SignalClear;
	SigSlot::Slotv0 SlotClear;

	SigSlot::Slotv2< const std::string &, CLAM::Processing *  > SlotAddProcessing;
	SigSlot::Signalv2 < const std::string &, CLAM::Processing * > SignalAddProcessing;
	SigSlot::Slotv2< const std::string & , CLAMVM::ProcessingController * > SlotCreateProcessingPresentation;
	SigSlot::Slotv1< ProcessingPresentation* > SlotRemoveProcessing;
	SigSlot::Signalv1< const std::string & > SignalRemoveProcessing;

	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreatePortConnection;
	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreatePortConnection;
	SigSlot::Signalv2< const std::string &, const std::string &  > SignalRemovePortConnection;
	SigSlot::Slotv1< ConnectionPresentation * > SlotRemovePortConnection;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter * > SlotCreatePortConnectionPresentation;	
	
	SigSlot::Slotv2< const std::string &, const std::string &> SlotRemoveConnectionPresentation;

	SigSlot::Signalv2< const std::string &, const std::string & > SignalCreateControlConnection;
	SigSlot::Slotv2< const std::string &, const std::string & > SlotCreateControlConnection;
	SigSlot::Signalv2< const std::string &, const std::string & > SignalRemoveControlConnection;
	SigSlot::Slotv1< ConnectionPresentation *> SlotRemoveControlConnection;
	SigSlot::Slotv1< CLAMVM::ConnectionAdapter * > SlotCreateControlConnectionPresentation;
};

} // namespace NetworkGUI

#endif // __NETWORKPRESENTATION_HXX__
