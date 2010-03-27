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
#include "Component.hxx"
#include "Text.hxx"
#include <string>
#include <list>
#include <map>
#include <set>
//#include <utility>

namespace CLAM
{

class NetworkPlayer;
class FlowControl;
class ControlSink;
class ControlSource;

class InformationText
{
public:
	int x;
	int y;
	Text text;
};


class Network : public Component
{

public:
	// used types
	typedef std::list<std::string> NamesList;
	typedef std::map <std::string, Processing* > ProcessingsMap;
	typedef std::list<InPortBase *> InPortsList;

	typedef std::vector<Processing*> Processings;
	typedef std::vector<ControlSource*> ControlSources;
	typedef std::vector<ControlSink*> ControlSinks;

	typedef std::vector<InformationText*> InformationTexts;

	typedef struct { int x, y, width, height; } Geometry;
	typedef struct { Processing* processing; Geometry geometry; } ProcessingAndGeometry;
	typedef std::map <std::string, Geometry> ProcessingsGeometriesMap;
	typedef struct { std::string sourceName, sinkName; } Connection;
	typedef std::list<Connection> ConnectionsList;
	// attributes for canvas copy & paste
	typedef std::set<std::string> NamesSet;
public:
	// constructor / destructor
	Network();
	virtual ~Network();

	// Methods related to network itself
	const std::string& GetName() const { return _name; }
	void SetName( const std::string& name ) { _name=name; }

	const Text& GetDescription() const {return _description;};
	void SetDescription( const Text& description ) {_description=description;};

	virtual const char * GetClassName() const
	{
		return "Network";
	}

	// Methods related with player and flow
	bool IsStopped() const;
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsRealTime() const;
	void Start();
	void Stop();
	void Pause();
	void Do();
	/** Gets the ownership of the FlowControl passed. So it will be deleted by the destructor */
	void AddFlowControl( FlowControl* );
	void Clear();
	/** Set the object in charge of managing the processing thread. Receives property of the player. */
	void SetPlayer( NetworkPlayer* player);
	unsigned BackendBufferSize();
	unsigned BackendSampleRate();

	//! serialization methods
	virtual void StoreOn( Storage & storage) const;
	virtual void LoadFrom( Storage & storage);

	//! methods related to copy&paste processings from canvas
	bool UpdateSelections (const NamesList & processingsNamesList);
	void setPasteMode() { _setPasteMode=true; }

	//! canvas related geometries
	bool SetProcessingsGeometries (const ProcessingsGeometriesMap & processingsGeometries);
	const ProcessingsGeometriesMap GetAndClearGeometries();

	//! audio sources and sinks and control sources and sinks  order 
	const Geometry findProcessingGeometry(Processing* ) const;
	const Processings getOrderedProcessings(const std::string & type, bool horizontalOrder) const;
	const Processings getOrderedProcessingsByAttribute(const std::string & attribute, bool horizontalOrder=false) const;
	const ControlSinks getOrderedControlSinks() const;
	const ControlSources getOrderedControlSources() const;
	static const bool compareGeometriesUpperYThan(ProcessingAndGeometry &, ProcessingAndGeometry &);
	static const bool compareGeometriesUpperXThan(ProcessingAndGeometry &, ProcessingAndGeometry &);

/*// TODO: make individual geometries loadings/storings??
	const Geometry GetAndEraseGeometry(std::string name);*/
	//! methods related to connect/disconnect interface
	bool ConnectPorts( const std::string &, const std::string & );
	bool ConnectControls( const std::string &, const std::string & );
	bool DisconnectPorts( const std::string &, const std::string & );
	bool DisconnectControls( const std::string &, const std::string & );

	//! methods used to create processings and get them
	Processing& GetProcessing( const std::string & name ) const;
	//! add method using a pointer to a new processing
	void AddProcessing( const std::string & name, Processing* processing, const ProcessingConfig * config=0 );
	//! add method using a key to get the new processing from factory
	Processing & AddProcessing( const std::string & name, const std::string & key );
	std::string AddProcessing( const std::string& key );
	std::string GetUnusedName( const std::string& prefix, const bool cutOnLastSeparator=false, const std::string separator="_") const;
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

	// accessors to txt boxes
	void addInformationText(InformationText * informationText);
	void removeInformationText(InformationText * informationText);

	InformationTexts::iterator BeginInformationTexts();
	InformationTexts::iterator EndInformationTexts();
	InformationTexts::const_iterator BeginInformationTexts() const;
	InformationTexts::const_iterator EndInformationTexts() const;

	InPortBase & GetInPortByCompleteName( const std::string& ) const;
	OutPortBase & GetOutPortByCompleteName( const std::string& ) const;
	InControlBase & GetInControlByCompleteName( const std::string& ) const;
	OutControlBase & GetOutControlByCompleteName( const std::string& ) const;

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

    /** This method reports whether the network supports dynamic buffer-size host.*/
	bool SupportsVariableAudioSize() const;

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

	/**
	 * Has true if ports have the old name AudioIn or AudioOut
	 * and which connections are broken
	 */
	typedef std::pair<bool, std::string> ConnectionState; 
	ConnectionState GetConnectionReport() const;
	void ResetConnectionReport();

private:
	// fields
	std::string _name;
	Text _description;
	ProcessingsMap _processings;
	FlowControl* _flowControl;
	NetworkPlayer* _player;
	InformationTexts _informationTexts;

	// attributes for canvas copy & paste
	mutable NamesSet _selectedProcessings;
	bool _setPasteMode;
	
	bool HasSelectionAndContains(const std::string & name) const;

	// attributes for canvas processing geometries
	mutable ProcessingsGeometriesMap _processingsGeometries;
	void StringPairToInts(const std::string & geometryInString, int & a, int & b);
	const std::string IntsToString (const int & a, const int & b) const;

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
private:
	bool BrokenConnection( const std::string & producer, const std::string & consumer );
	std::vector<std::string> _brokenConnections;
};

}// namespace
#endif

