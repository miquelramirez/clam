/*
 * Copyright (c) 2008 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef _BaseNetwork_hxx_
#define _BaseNetwork_hxx_

#include "Processing.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>
#include <list>
#include <map>
#include <set>
#include "Component.hxx"

namespace CLAM
{
// abstract clase of a base Network
class BaseNetwork : public Component
{
public:
	// used types
	typedef std::list<std::string> NamesList;
	typedef std::map <std::string, Processing* > ProcessingsMap;
	typedef std::map<std::string, NamesList > ConnectionsMap;
	typedef std::list<InPortBase *> InPortsList;

	typedef struct
	{
		int x,y,width,height;
	} Geometry;
	typedef std::map <std::string, Geometry> ProcessingsGeometriesMap;

	// constructor / destructor
	BaseNetwork() {}
	virtual ~BaseNetwork() {}

	// serialization methods
	virtual void StoreOn( Storage & storage) const = 0;
	virtual void LoadFrom( Storage & storage) = 0;

	// Methods related to network itself
	virtual const std::string& GetName() const = 0;
	virtual void SetName( const std::string& name ) = 0;
	virtual const std::string & GetNetworkId(const Processing * proc) const = 0;

	// pure virtual methods:
	virtual const char * GetClassName() const = 0;
	virtual void Clear() = 0;

	// accessors to nodes and processing
	virtual ProcessingsMap::iterator BeginProcessings() = 0; // { return _processings.begin(); }
	virtual ProcessingsMap::iterator EndProcessings() = 0;// { return _processings.end(); }
	virtual ProcessingsMap::const_iterator BeginProcessings() const = 0; //{ return _processings.begin(); }
	virtual BaseNetwork::ProcessingsMap::const_iterator EndProcessings() const = 0; //{ return _processings.end(); }

	virtual NamesList  GetInPortsConnectedTo( const std::string & producer ) const = 0;
	virtual NamesList  GetInControlsConnectedTo( const std::string & producer ) const = 0;
	virtual InPortsList GetInPortsConnectedTo( OutPortBase & producer ) const = 0;
	virtual std::string GetConnectorIdentifier( const std::string& ) const = 0;
	virtual std::string GetProcessingIdentifier( const std::string& ) const = 0;
	virtual InPortBase & GetInPortByCompleteName( const std::string & name ) const = 0;
	virtual OutPortBase & GetOutPortByCompleteName( const std::string & name ) const = 0;
	virtual InControl & GetInControlByCompleteName( const std::string & name ) const = 0;
	virtual OutControl & GetOutControlByCompleteName( const std::string & name ) const = 0;

	// methods related to connect/disconnect interface
	virtual bool ConnectPorts( const std::string &, const std::string & ) = 0;
	virtual bool ConnectControls( const std::string &, const std::string & ) = 0;
	virtual bool DisconnectPorts( const std::string &, const std::string & ) = 0;
	virtual bool DisconnectControls( const std::string &, const std::string & ) = 0;

	virtual const ConnectionsMap getConnectionsMap() const = 0;
	virtual void RefreshConnections() = 0;

	/**
	 * Returns an string the full name of the unconnected inports.
	 * Don't rely on the format because is subject to change.
	 */
	virtual std::string GetUnconnectedInPorts() const = 0;

	//! methods used to create processings and get them
	virtual Processing& GetProcessing( const std::string & name ) const = 0;
	//! add method using a pointer to a new processing
	virtual void AddProcessing( const std::string &, Processing* ) = 0;
	//! add method using a key to get the new processing from factory
	virtual void AddProcessing( const std::string & name, const std::string & key ) = 0;
	virtual std::string AddProcessing( const std::string& key ) = 0;
	virtual std::string GetUnusedName( const std::string& prefix ) const = 0;
	virtual void RemoveProcessing ( const std::string & ) = 0;
	virtual bool HasProcessing( const std::string & name ) const = 0;
	virtual bool RenameProcessing( const std::string & oldName, const std::string & newName ) = 0;

	/** Tells whether the network is ready to rock. A network is ready when:
	 * - it contains any processing,
	 * - all processings are properly configured, and
	 * - all in ports are connected,
	 */ 
	virtual bool IsReady() const = 0;
	/** Returns true when it has no processings */
	virtual bool IsEmpty() const = 0;
	/** Returns true when some processing is misconfigured */
	virtual bool HasMisconfiguredProcessings() const = 0;
	/** Returns true when a processing has an inport that is not connected */
	virtual bool HasUnconnectedInPorts() const = 0;
	/** Tell whether the network contains any processing which
	 * limits the cpu usage such as file or device interfaces.
	 * This is needed when executing a network in a real time process.
	 */
	virtual bool HasSyncSource() const = 0;
	/** It configures the processing with the given processing id and config object and
	 *  notifies this to the network. So use this method instead of getting the processing
	 *  with GetProcessing and calling Configure on it. **/
	virtual bool ConfigureProcessing( const std::string &, const ProcessingConfig & ) = 0;
	/** Calls Configure() for each processing with its current configuration  */
	virtual void ReconfigureAllProcessings() = 0;
	/**
	 * Returns an string containing configuration errors.
	 * Don't rely on the format because is subject to change.
	 */
	virtual std::string GetConfigurationErrors() const = 0;
	// methods related to copy&paste processings from canvas
	virtual bool UpdateSelections (const NamesList & processingsNamesList) = 0;
	virtual void setPasteMode() = 0; // { _setPasteMode=true; }
	// canvas related geometries
	virtual bool SetProcessingsGeometries (const ProcessingsGeometriesMap & processingsGeometries) = 0;
	virtual const ProcessingsGeometriesMap GetAndClearGeometries() = 0;
/*// TODO: make individual geometries loadings/storings??
	const Geometry GetAndEraseGeometry(std::string name);*/

protected:
	static std::size_t PositionOfLastIdentifier( const std::string& str)
	{
		std::size_t result = str.find_last_of( NamesIdentifiersSeparator() );
		CLAM_ASSERT( result!=std::string::npos, "Malformed port/control name. It should be ProcessingName.[Port/Control]Name");
		return result;
	}
	static std::size_t PositionOfProcessingIdentifier( const std::string& str)
	{
		std::size_t endPos = PositionOfLastIdentifier(str)-1;
		std::size_t	last_ofResult = str.find_last_of( NamesIdentifiersSeparator(), endPos );
		return last_ofResult == std::string::npos ? 0 : last_ofResult+1;
	}
	static char NamesIdentifiersSeparator()
	{
		return '.';
	}
	// attributes for canvas copy & paste
	typedef std::set<std::string> NamesSet;
	virtual bool HasSelectionAndContains(const std::string & name) const = 0;
};
}
#endif
