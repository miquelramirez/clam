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



#ifndef _Network_hxx_
#define _Network_hxx_

#include "Processing.hxx"
#include "Node.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "OutPortTmpl.hxx"
#include "CircularStreamImpl.hxx"
#include "NodeTmpl.hxx"
#include <string>
#include <list>
#include <map>

#include "Audio.hxx"
#include "Component.hxx"
#include "Storage.hxx"

namespace CLAM
{

class FlowControl;

class Network : public Component
{
public:
	typedef std::map< std::string, Processing* > ProcessingsMap;
	typedef std::list< NodeBase* > Nodes;
	typedef std::list<std::string> NamesList;
	typedef std::list<InPort *> InPortsList;
	
	// constructor / destructor
	Network();
	virtual ~Network();

	// Methods related to network itself
	const std::string& GetName() const { return mName; }
	void SetName( const std::string& name ) { mName=name; }
	virtual const char * GetClassName() const
	{
		return "Network";
	}
	void Start();
	void Stop();
	void DoProcessings();
	void AddFlowControl( FlowControl* );
	void ConfigureAllNodes();
	void Clear();
	// serialization methods
	virtual void StoreOn( Storage & storage) const;
	virtual void LoadFrom( Storage & storage);
	


	// methods related to connect/disconnect interface
	bool ConnectPorts( const std::string &, const std::string & );
	bool ConnectControls( const std::string &, const std::string & );
	bool DisconnectPorts( const std::string &, const std::string & );
	bool DisconnectControls( const std::string &, const std::string & );
	void DisconnectAllPorts();

	// methods used to create processings and get them
	Processing& GetProcessing( const std::string & name ) const;
	// add method using a pointer to a new processing
	void AddProcessing( const std::string &, Processing* );
	// add method using a key to get the new processing from factory
	void AddProcessing( const std::string & name, const std::string & key );
	void RemoveProcessing ( const std::string & );
	bool HasProcessing( const std::string & name ) const;
	void ChangeKeyMap( const std::string & oldName, const std::string & newName );	
	
	const std::string & GetNetworkId(const Processing * proc) const;

	
	
	// accessors to nodes and processing
	ProcessingsMap::iterator BeginProcessings();
	ProcessingsMap::iterator EndProcessings();
	ProcessingsMap::const_iterator BeginProcessings() const;
	ProcessingsMap::const_iterator EndProcessings() const;

	InPort & GetInPortByCompleteName( const std::string& ) const;
	OutPort & GetOutPortByCompleteName( const std::string& ) const;
	InControl & GetInControlByCompleteName( const std::string& ) const;
	OutControl & GetOutControlByCompleteName( const std::string& ) const;

	NamesList GetInPortsConnectedTo( const std::string & ) const;
	NamesList GetInControlsConnectedTo( const std::string & ) const;
	InPortsList GetInPortsConnectedTo( OutPort & ) const;

protected:
	NodeBase & GetNodeAttachedTo(OutPort & );
private:
	
	// fields
	std::string mName;
	ProcessingsMap mProcessings;
	Nodes mNodes;
	Nodes mNodesToConfigure;

	// helpers
	Nodes::iterator BeginNodes();
	Nodes::iterator EndNodes();
	Nodes::const_iterator BeginNodes() const;
	Nodes::const_iterator EndNodes() const;

	void AssertFlowControlNotNull() const;
	static std::size_t PositionOfLastIdentifier( const std::string& );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& ) const;
	std::string GetProcessingIdentifier( const std::string& ) const;
	static char NamesIdentifiersSeparator();
	FlowControl* mFlowControl;

	/**this method is provisional, because Network may need non-audio nodes.
	 * Thus the factory method should be a (virtual) method of OutPort, implemented
	 * in the concrete class.
	 * \todo the motivation for this kludge is in order to avoid the coupling between
	 * ports and the nodes stuff, since VC6 doesn't compiles all of it.
	 */
	NodeBase* CreateAudioNodeWithDefaultStreamBuffer();
	
};

}// namespace
#endif
