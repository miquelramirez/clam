#include "ControlExpression.hxx"
#include <CLAM/ProcessingFactory.hxx>

#include "main.hpp"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlExpression",
		"category", "[plugin] Expression compiler",
		"description", "ControlExpression",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlExpression> reg = metadata;
}

ControlExpression::ControlExpression( const Config& cfg ) 
{ 
	Configure( cfg );
}

bool ControlExpression::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( _config, cfg );
	
	typedef std::string::const_iterator iterator_type;
	typedef program<iterator_type> program;
	
	program prog(_code); 
	std::string const& source_code = _config.GetExpression();
	
	if (!compile(prog, source_code))
	{
		return AddConfigErrorMessage(
			"Sorry, could not compile your function: "
			+ source_code);
	}
	
	// use arity (and variable names??) from expression for portnr and name
	std::string fmain("main");
	std::string::iterator fbegin = fmain.begin();
	function_info* f = prog.functions.lookup->find(fbegin, fmain.end());
	if (f == 0)
		return AddConfigErrorMessage("Error: main function not defined");
	
	//int nrOfInPorts = _config.GetNrOfInports();
	int nrOfInPorts = f->arity;
	
	_inControls.clear();
	for (int i = 0; i < nrOfInPorts; ++i)
	{
		char id = 97 + i; // start with 'a'
		std::stringstream ss;
		ss << id;
		_inControls.push_back(InControlPtr(new FloatInControl(ss.str(), this)));
	}
	
	_inControlValues.resize(nrOfInPorts);
	
	//int nrOfOutPorts = _config.GetNrOfOutports();
	int nrOfOutPorts = 1;

	_outControls.clear();
	for (int i = 0; i < nrOfOutPorts; ++i)
	{
		std::stringstream ss;
		ss << "result";
		_outControls.push_back(OutControlPtr(new FloatOutControl(ss.str(), this)));
	}
	
	return true; 		
}

bool ControlExpression::Do()
{
	for (unsigned i = 0; i < _inControls.size(); ++i)
		_inControlValues[i] = _inControls[i]->GetLastValue();
	
	_mach.stack = _inControlValues;
	float r = _mach.execute(_code, _code.begin(), _mach.stack.begin());
	
	_outControls[0]->SendControl(r);
	
	return true;
}

}

