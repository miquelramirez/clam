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
#include "NetworkModel.hxx"
#include "ProcessingController.hxx"
#include "ConnectionAdapter.hxx"

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
	class NetworkController : public ModelController, public NetworkModel
	{
		typedef std::map< std::string , std::string  > ConnectionsMap;
		typedef std::list< std::string > ProcessingsList;
	private:		

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
		std::list<ProcessingController*> mProcessingControllers;
		typedef std::list<ProcessingController*>::iterator ProcessingControllerIterator;
		std::list<ConnectionAdapter*> mConnectionAdapters;
		typedef std::list<ConnectionAdapter*>::iterator ConnectionAdapterIterator;

		void OnNewPortConnectionFromGUI( const std::string &, const std::string & );
		void OnNewControlConnectionFromGUI( const std::string &, const std::string & );
		void OnNewChangeState( bool);
		void OnRemovePortConnectionFromGUI( const std::string &, const std::string & );
		void OnRemoveControlConnectionFromGUI( const std::string &, const std::string & );
		void OnRemoveProcessingFromGUI( const std::string & );
		void OnLoadNetwork( const std::string & );
		void OnSaveNetwork( const std::string & );
		void OnClear();

		// helper methods
		void ConnectPorts( const std::string & , const std::string & );
		void ConnectControls( const std::string & , const std::string & );
		void DisconnectPorts( const std::string & , const std::string & );
		void DisconnectControls( const std::string & , const std::string & );
		void RemoveProcessingFromNetwork( const std::string & );
		void ExecuteEvents();
	public:
		NetworkController();
		virtual ~NetworkController();
		virtual const char* GetClassName() const
		{
			return "NetworkController";
		}
		void AddProcessing( const std::string & , CLAM::Processing *  );
		void NewProcessingFromGUI( const std::string & , CLAM::Processing * );
	
		virtual bool Publish();
		virtual bool Update();
		virtual bool BindTo( CLAM::Network&  );
	};
} // namespace CLAMVM

#endif //_NETWORKCONTROLLER_HXX_
