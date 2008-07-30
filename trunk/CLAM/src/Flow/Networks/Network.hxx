/*
 * Copyright (c) 2001-2007 MUSIC TECHNOLOGY GROUP (MTG)
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



#ifndef _Network_hxx_
#define _Network_hxx_

#include "Processing.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>
#include <list>
#include <map>
#include <set>

//#include "Component.hxx"
//#include "Storage.hxx"
#include "BaseNetwork.hxx"

namespace CLAM
{
class NetworkPlayer;
class FlowControl;

class Network : public BaseNetwork
{

public:
	// constructor / destructor
	Network();
	virtual ~Network();

	// Methods related to network itself
	const std::string& GetName() const { return _name; }
	void SetName( const std::string& name ) { _name=name; }
	virtual const char * GetClassName() const
	{
		return "Network";
	}
	bool IsStopped() const;
	void Start();
	void Stop();
	void Do();
	/** Gets the ownership of the FlowControl passed. So it will be deleted by the destructor */
	void AddFlowControl( FlowControl* );
	void Clear();
	/** Set the object in charge of managing the processing thread. Receives property of the player. */
	void SetPlayer( NetworkPlayer* player);
	unsigned BackendBufferSize();
	unsigned BackendSampleRate();
	// serialization methods
	virtual void StoreOn( Storage & storage) const;
	virtual void LoadFrom( Storage & storage);

	// methods related to copy&paste processings from canvas
	bool UpdateSelections (const NamesList & processingsNamesList);
	void setPasteMode() { _setPasteMode=true; }

	// canvas related geometries
	bool SetProcessingsGeometries (const ProcessingsGeometriesMap & processingsGeometries);
	const ProcessingsGeometriesMap GetAndClearGeometries();

/*// TODO: make individual geometries loadings/storings??
	const Geometry GetAndEraseGeometry(std::string name);*/

	// methods related to connect/disconnect interface
	bool ConnectPorts( const std::string &, const std::string & );
	bool ConnectControls( const std::string &, const std::string & );
	bool DisconnectPorts( const std::string &, const std::string & );
	bool DisconnectControls( const std::string &, const std::string & );

	//! methods used to create processings and get them
	Processing& GetProcessing( const std::string & name ) const;
	//! add method using a pointer to a new processing
	void AddProcessing( const std::string &, Processing* );
	//! add method using a key to get the new processing from factory
	void AddProcessing( const std::string & name, const std::string & key );
	std::string AddProcessing( const std::string& key );
	std::string GetUnusedName( const std::string& prefix ) const;
	void RemoveProcessing ( const std::string & );
	bool HasProcessing( const std::string & name ) const;
	/** It configures the processing with the given processing id and config object and
	 *  notifies this to the network. So use this method instead of getting the processing
	 *  with GetProcessing and calling Configure on it. **/
	bool ConfigureProcessing( const std::string &, const ProcessingConfig & );
	/** Calls Configure() for each processing with its current configuration  */
	void ReconfigureAllProcessings();
	bool RenameProcessing( const std::string & oldName, const std::string & newName );	
	
	const std::string & GetNetworkId(const Processing * proc) const;
	
	// accessors to nodes and processing
	ProcessingsMap::iterator BeginProcessings();
	ProcessingsMap::iterator EndProcessings();
	ProcessingsMap::const_iterator BeginProcessings() const;
	ProcessingsMap::const_iterator EndProcessings() const;

	InPortBase & GetInPortByCompleteName( const std::string& ) const;
	OutPortBase & GetOutPortByCompleteName( const std::string& ) const;
	InControl & GetInControlByCompleteName( const std::string& ) const;
	OutControl & GetOutControlByCompleteName( const std::string& ) const;

	NamesList GetInPortsConnectedTo( const std::string & ) const;
	NamesList GetInControlsConnectedTo( const std::string & ) const;
	InPortsList GetInPortsConnectedTo( OutPortBase & ) const;

	std::string GetConnectorIdentifier( const std::string& ) const;
	std::string GetProcessingIdentifier( const std::string& ) const;

	/** Tells whether the network is ready to rock. A network is ready when:
	 * - it contains any processing,
	 * - all processings are properly configured, and
	 * - all in ports are connected,
	 */ 
	bool IsReady() const;

	/** Returns true when it has no processings */
	bool IsEmpty() const;

	/** Returns true when some processing is misconfigured */
	bool HasMisconfiguredProcessings() const;

	/** Returns true when a processing has an inport that is not connected */
	bool HasUnconnectedInPorts() const;

	/** Tell whether the network contains any processing which
	 * limits the cpu usage such as file or device interfaces.
	 * This is needed when executing a network in a real time process.
	 */
	bool HasSyncSource() const;

	/**
	 * Returns an string containing configuration errors.
	 * Don't rely on the format because is subject to change.
	 */
	std::string GetConfigurationErrors() const;
	/**
	 * Returns an string the full name of the unconnected inports.
	 * Don't rely on the format because is subject to change.
	 */
	std::string GetUnconnectedInPorts() const;

private:
	
	// fields
	std::string _name;
	ProcessingsMap _processings;

	FlowControl* _flowControl;
	NetworkPlayer* _player;
	
	// attributes for canvas copy & paste
	mutable NamesSet _selectedProcessings;
	bool _setPasteMode;
	
	bool HasSelectionAndContains(const std::string & name) const;

	// attributes for canvas processing geometries
	mutable ProcessingsGeometriesMap _processingsGeometries;
	void StringPairToInts(const std::string & geometryInString, int & a, int & b);
	const std::string IntsToString (const int & a, const int & b) const;


};

}// namespace
#endif

