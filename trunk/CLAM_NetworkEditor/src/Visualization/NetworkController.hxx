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

#ifndef _NETWORKCONTROLLER_HXX_
#define _NETWORKCONTROLLER_HXX_

#include "ModelController.hxx"
#include "ProcessingController.hxx"
#include "ConnectionAdapter.hxx"

//#include "Signalv0.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"


#include <list>
#include <string>
#include <map>

#include "Thread.hxx"



namespace CLAM
{
	class Network;
	class Processing;
}

namespace CLAMVM
{
	class NetworkController : public ModelController
	{
	public:
		typedef std::map< std::string , std::string  > ConnectionsMap;
		typedef std::map< std::string , ProcessingController*  > ProcessingControllersMap;
		typedef ProcessingControllersMap::iterator ProcessingControllersMapIterator;
		typedef std::list< std::string > ProcessingsList;
	protected:		

		// multithread related
		CLAM::Thread mThread;
		void ProcessingLoop();
		bool mLoopCondition;
		ConnectionsMap mPortsToConnect;
		ConnectionsMap mPortsToDisconnect;
		ConnectionsMap mControlsToConnect;
		ConnectionsMap mControlsToDisconnect;

		ProcessingsList mProcessingsToRemove;

		CLAM::Network* mObserved;
		ProcessingControllersMap mProcessingControllers;
		std::list<ConnectionAdapter*> mConnectionAdapters;
		typedef std::list<ConnectionAdapter*>::iterator ConnectionAdapterIterator;

		void CreatePortConnection( const std::string &, const std::string & );
		void ExecuteCreatePortConnection( const std::string & , const std::string & );
		void ExecuteRemovePortConnection( const std::string & , const std::string & );
		void RemovePortConnection( const std::string &, const std::string & );
		
		void CreateControlConnection( const std::string &, const std::string & );
		void ExecuteCreateControlConnection( const std::string & , const std::string & );
		void ExecuteRemoveControlConnection( const std::string & , const std::string & );
		void RemoveControlConnection( const std::string &, const std::string & );

		void ChangeState( bool);
		void LoadNetwork( const std::string & );
		void SaveNetwork( const std::string & );
		
		void Clear();
	
		void RemoveProcessing( const std::string & );
		void ExecuteRemoveProcessing( const std::string & );
		
		void ExecuteEvents();
	public:
		NetworkController();
		virtual ~NetworkController();
		virtual const char* GetClassName() const
		{
			return "NetworkController";
		}
		std::string GetName();
		ProcessingController* CreateProcessingController( const std::string & , CLAM::Processing *  );
		void AddProcessing( const std::string & , CLAM::Processing * );
		ProcessingControllersMapIterator BeginProcessingControllers()
		{
			return mProcessingControllers.begin();
		}
		ProcessingControllersMapIterator EndProcessingControllers()
		{
			return mProcessingControllers.end();
		}

		virtual bool Publish();
		virtual bool Update();
		virtual bool BindTo( CLAM::Network&  );

	public:
		SigSlot::Slotv1< bool > SlotChangeState;
		
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter * > SignalCreatePortConnectionPresentation;
		SigSlot::Slotv2< const std::string &, const std::string & > SlotRemovePortConnection;
		SigSlot::Slotv2< const std::string &, const std::string& > SlotCreatePortConnection;
		
		SigSlot::Signalv2< const std::string &, const std::string & > SignalRemoveConnectionPresentation;
		
		SigSlot::Signalv1< CLAMVM::ConnectionAdapter * > SignalCreateControlConnectionPresentation;
		SigSlot::Slotv2< const std::string &, const std::string & > SlotRemoveControlConnection;
		SigSlot::Slotv2< const std::string &, const std::string& > SlotCreateControlConnection;

		SigSlot::Slotv2< const std::string &, CLAM::Processing *  > SlotAddProcessing;
		SigSlot::Slotv1< const std::string & > SlotRemoveProcessing;
		SigSlot::Signalv2< const std::string &, CLAMVM::ProcessingController* > SignalCreateProcessingPresentation;

		SigSlot::Slotv1< const std::string & > SlotLoadNetwork;
		SigSlot::Slotv1< const std::string & > SlotSaveNetwork;
		SigSlot::Slotv0 SlotClear;
	};
} // namespace CLAMVM

#endif //_NETWORKCONTROLLER_HXX_
