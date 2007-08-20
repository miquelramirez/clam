/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef OSCControlDispatcher_hxx
#define OSCControlDispatcher_hxx


//CLAM dependencies
#include "Network.hxx"
#include "BasicFlowControl.hxx"
#include "Thread.hxx"

//OSC dependencies
#include <oscpack/ip/NetworkingUtils.h>
#include <oscpack/ip/UdpSocket.h>
#include <oscpack/ip/TimerListener.h>
#include <oscpack/osc/OscPacketListener.h>
#include <oscpack/osc/OscReceivedElements.h>

//Other dependencies
#include <queue>
#include <list>
#include <string>
#include <vector>
#include <map>

namespace CLAM
{

/** 
 * This class provides a generic interface to access OSC messages. When it receives
 * an OSC message, it stores the message values as floats in a vector, and saves
 * them internally by tying them to the message name in a map. When a client
 * would like to access the most recent value, he queries the map using the message
 * name.
 * 
 * To use this class, you will need to have the oscpack library installed.
 * It can be downloaded at: http://www.audiomulch.com/~rossb/code/oscpack/
 * And you will need to compile CLAM with the flag with_osc=true 
 *
 * For more information on OSC, see http://www.cnmat.berkeley.edu/OpenSoundControl/
 * 
 * \author greg kellum [gkellum@iua.upf.edu] 08/20/2007
 * \since CLAM v1.1
 *
 */
class OSCSource : public osc::OscPacketListener
{
	virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );

	void CreateThread();

	int mPort;
	CLAM::Thread mThread;
	UdpListeningReceiveSocket *mReceiveSocket;
	bool mIsListening;
	std::queue<std::string> mMessageLog;

public:
	/// The default constructor listens for OSC messages on port 7000
	OSCSource();
	OSCSource(int portNumber);
	~OSCSource();

	/**
	* This method allows clients to access the most recent values for a particular
	* target name. It should be queried frequently, i.e. once per every control rate
	* cycle by the client.
	*
	* \arg targetName the OSC style name of the message, e.g. /ebowSynthesizer
	* \returns a vector containing all the values in the message as floats.
	*/
	std::vector<float>* GetLastValuesForTarget(std::string& targetName);

	/**
	* Call this after object creation to start the OSCSource listening for messages
	* on a thread.
	*/
	void Start()
	{
		if ( IsListening() )
			return;
		mThread.Start();
		mIsListening = true;
	}

	/**
	* Stop the OSCSource listener thread.
	*/
	void Stop()
	{
		if (not IsListening() )
			return;
		mReceiveSocket->AsynchronousBreak();
		mThread.Stop();
		mIsListening = false;
	}

	/**
	* \returns true if the thread is listening on a port
	*/
	bool IsListening() const { return mIsListening; }

	const std::string GetLogMessage();

private:
	void AddLogMessage(const std::string& message);

	std::map<std::string, std::vector<float> > oscValuesMap;

};

} // namespace CLAM


#endif // OSCControlDispatcher_hxx

