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

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "Slotv3.hxx"


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
	
		CLAM::Thread mThread;
		/** 
		 * Main loop of the application, where DoProcessings method from network
		 * is executed and all the events are checked calling ExecuteEvents.
		 * @see ExecuteEvents
		 */
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

		/** 
		 * This method creates a processing controller for a concrete Processing and returns it in order to
		 * create a processing presentation attached to this instance of controller
		 */
		ProcessingController* CreateProcessingController( const std::string & , CLAM::Processing *  );

		/** 
		 * This method creates a port connection adapter for a link between ports and returns it in order to
		 * create a port connection presentation attached to this instance of adapter
		 */
		ConnectionAdapter * CreatePortConnectionAdapter( const CLAM::OutPort & out, const CLAM::InPort & in );
		/** 
		 * This method creates a control connection adapter for a link between controls and returns it in order to
		 * create a control connection presentation attached to this instance of adapter
		 */
		ConnectionAdapter * CreateControlConnectionAdapter( const CLAM::OutControl & out, const CLAM::InControl & in );

		/** 
		 * When a connection is created from GUI, this method is called. It leaves the event in a list to execute if the 
		 * audio thread is running, or executes the creation, calling ExecuteCreatePortConnection.
		 * @see ExecuteCreatePortConnection
		 */
		void CreatePortConnection( const std::string &, const std::string & );
	
		/** 
		 * When a connection is removed from GUI, this method is called. It leaves the event in a list to execute if the 
		 * audio thread is running, or executes the creation, calling ExecuteRemovePortConnection.
		 * @see ExecuteRemovePortConnection
		 */
		void RemovePortConnection( const std::string &, const std::string & );

		/** 
		 * In this method the connection is created at network level, and if it's successful a signal is emitted to the GUI in order to
		 * create a port connection presentation
		 */
		void ExecuteCreatePortConnection( const std::string & , const std::string & );
		
		/** 
		 * In this method the connection is removed at network level, and if it's successful a signal is emitted to the GUI in order to
		 * remove the proper port connection presentation.
		 */
		void ExecuteRemovePortConnection( const std::string & , const std::string & );

		
		/** 
		 * When a connection is created from GUI, this method is called. It leaves the event in a list to execute if the 
		 * audio thread is running, or executes the creation, calling ExecuteCreateControlConnection.
		 * @see ExecuteCreateControlConnection
		 */
		void CreateControlConnection( const std::string &, const std::string & );
	
		/** 
		 * When a connection is removed from GUI, this method is called. It leaves the event in a list to execute if the 
		 * audio thread is running, or executes the creation, calling ExecuteRemoveControlConnection.
		 * @see ExecuteRemoveControlConnection
		 */	
		void RemoveControlConnection( const std::string &, const std::string & );

		/** 
		 * In this method the connection is created at network level, and if it's successful a signal is emitted to the GUI in order to
		 * create a control connection presentation
		 */
		void ExecuteCreateControlConnection( const std::string & , const std::string & );

		/** 
		 * In this method the connection is removed at network level, and if it's successful a signal is emitted to the GUI in order to
		 * remove the proper control connection presentation.
		 */
		void ExecuteRemoveControlConnection( const std::string & , const std::string & );
	
		void ChangeState( bool);
		void LoadNetwork( const std::string & );
		void SaveNetwork( const std::string & );
		
		void Clear();
			
		/** 
		 * When a processing is removed from GUI, this method is called. It leaves the event in a list to execute if the 
		 * audio thread is running, or executes the creation, calling ExecuteRemoveProcessing
		 * @see ExecuteRemoveProcessing
		 */	
		void RemoveProcessing( const std::string & );

		/** 
		 * In this method all the connections of a concrete processing are removed, and after this step has been done, the processing is removed
		 * from network.
		 * @see RemoveAllPortConnections, @see RemoveAllControlConnections
		 */
		void ExecuteRemoveProcessing( const std::string & );

		/** 
		 * This method is called when a processing has been created from gui. In this case, the processing is added to the network, its
		 * controller created and a signal emitted to create the proper presentation.
		 */
		void AddProcessing( const std::string & , CLAM::Processing * );
		/** 
		 * This method is called when a processing name is changed from gui. It executes the change inside NetworkController and
		 * passes the change to Network
		 * @see ChangeKeyMap
		 * @see ChangeProcessingNameInsideConnections
		 */
		void ProcessingNameChanged( const std::string &, ProcessingController * );

		/* 
		 * Using this function from ProcessingNameChanged the name of the controller is changed in the ProcessingControllersMap.
		 * It returns false if the name is duplicated, in order to not modify gui
		 * @see ProcessingNameChanged
		 */
		bool ChangeKeyMap( const std::string & oldName , const std::string & newName );
		
		/*
		 * This method disconnects all the ports of a concrete processing, removing in the process all the presentation of its connections.
		 */
		void RemoveAllPortConnections( const std::string &  );

		/*
		 * This method disconnects all the controls of a concrete processing, removing in the process all the presentation of its connections.
		 */
		void RemoveAllControlConnections( const std::string & );

		/*
		 * This method, called from ProcessingLoop, executes all the events related to creation/destruction of processings/connections
		 * in a thread-safe way.
		 */		
		void ExecuteEvents();

		void RemoveAllConnections( CLAM::Processing * );
		void RebuildProcessingPresentationAttachedTo( ProcessingController *, CLAM::Processing * );

	public:
		NetworkController();
		virtual ~NetworkController();
		virtual const char* GetClassName() const
		{
			return "NetworkController";
		}
		std::string GetName();

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
		SigSlot::Slotv2< const std::string &, ProcessingController * > SlotProcessingNameChanged;
		SigSlot::Signalv2< const std::string &, const std::string & > SignalChangeConnectionPresentationNames;

		SigSlot::Signalv2< const std::string &, CLAMVM::ProcessingController* > SignalCreateProcessingPresentation;
		SigSlot::Signalv2< const std::string &, ProcessingController *> SignalRebuildProcessingPresentationAttachedTo;
		SigSlot::Slotv1< CLAM::Processing * > SlotRemoveAllConnections;
		SigSlot::Slotv2< ProcessingController *, CLAM::Processing * > SlotRebuildProcessingPresentationAttachedTo;

		SigSlot::Slotv1< const std::string & > SlotLoadNetwork;
		SigSlot::Slotv1< const std::string & > SlotSaveNetwork;
		SigSlot::Slotv0 SlotClear;
		
	};
} // namespace CLAMVM

#endif //_NETWORKCONTROLLER_HXX_
