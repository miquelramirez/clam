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
	
	// constructor / destructor
	Network();
	virtual ~Network();
	
	const std::string& GetName() const { return mName; }
	void SetName( const std::string& name ) { mName=name; }
	Processing& GetProcessing( const std::string & name );
	void AddProcessing( const std::string &, Processing* );
	void RemoveProcessing ( const std::string & );
	bool HasProcessing( const std::string & name );
	bool ConnectPorts( const std::string &, const std::string & );
	bool ConnectControls( const std::string &, const std::string & );
	bool DisconnectPorts( const std::string &, const std::string & );
	bool DisconnectControls( const std::string &, const std::string & );
	void DisconnectAllPorts();
	
	void Start();
	void Stop();
	void DoProcessings();

	void ConfigureAllNodes();

	void AddFlowControl( FlowControl* );
	void Clear();

	// accessors to nodes and processing
	ProcessingsMap::iterator BeginProcessings();
	ProcessingsMap::iterator EndProcessings();
	ProcessingsMap::const_iterator BeginProcessings() const;
	ProcessingsMap::const_iterator EndProcessings() const;
	Nodes::iterator BeginNodes();
	Nodes::iterator EndNodes();
	Nodes::const_iterator BeginNodes() const;
	Nodes::const_iterator EndNodes() const;
	InPort & GetInPortByCompleteName( const std::string& );
	OutPort & GetOutPortByCompleteName( const std::string& );
	InControl & GetInControlByCompleteName( const std::string& );
	OutControl & GetOutControlByCompleteName( const std::string& );
	NamesList GetInPortsConnectedTo( const std::string & );
	NamesList GetInControlsConnectedTo( const std::string & );
	// serialization methods
	virtual void StoreOn( Storage & storage);
	virtual void LoadFrom( Storage & storage);
	virtual const char * GetClassName() const
	{
		return "Network";
	}
	

protected:
	NodeBase & GetNodeAttachedTo(OutPort & );
	const std::string & GetNetworkId(const Processing * proc);
private:
	
	// fields
	std::string mName;
	ProcessingsMap mProcessings;
	Nodes mNodes;
	Nodes mNodesToConfigure;

	// helpers
	void AssertFlowControlNotNull() const;
	static std::size_t PositionOfLastIdentifier( const std::string& );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );
	std::string GetProcessingIdentifier( const std::string& );
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
