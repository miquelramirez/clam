/*
 */

#ifndef _ControlExpression_
#define _ControlExpression_

#include <CLAM/DataTypes.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/TextBlock.hxx>

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "definitions.hpp"

namespace CLAM
{

class ControlExpression : public Processing
{
private:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config, 1, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, TextBlock, Expression);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetExpression( 
				"float main(a, b, c)\n"
				"{\n"
				"    return (a + b) - c;\n"
				"}\n"
			
			);
		}
	};
	
	Config _config;
	
	typedef boost::shared_ptr<FloatInControl> InControlPtr;
	typedef std::vector<InControlPtr> InControls;
	InControls _inControls;
	
	typedef boost::shared_ptr<FloatOutControl> OutControlPtr;
	typedef std::vector<OutControlPtr> OutControls;
	OutControls _outControls;	

	vmachine _mach;             
	code_t _code;   

protected:
	bool ConcreteConfigure( const ProcessingConfig& cfg ); 

public:
	ControlExpression( const Config& cfg = Config());

	const char *GetClassName() const { return "ControlExpression"; }
	const ProcessingConfig& GetConfig() const { return _config; }
	
	bool Do();
};

}

#endif

