/*
 * Copyright (c) 2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#include "ControlExpression.hxx"
#include <CLAM/ProcessingFactory.hxx>

#include "main.hpp"
#include <vector>

namespace CLAM
{
	
namespace 
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
	
	code_t code;
	program prog(code); 
	std::string const& source_code = _config.GetExpression();
	
	if (!compile(prog, source_code))
	{
		return AddConfigErrorMessage("Sorry, could not compile your function");
	}
	
	// assign the new byte code stack so we can use it in Do()
	_code = code;
	
	// use arity and variable names from expression for portnr and name
	var_names_t names = prog.get_var_names();
	
	//  we might use the (main) function name as out port name
	if (_outControls.size() == 0)
		_outControls.push_back(OutControlPtr(new FloatOutControl("result", this)));
	
	if (_inControls.size() == names.size()) // what if the names changed?
		return true;
	
	unsigned oldSize = _inControls.size();
	_inControls.resize(names.size());

	for (unsigned port = oldSize; port < names.size(); ++port)
		_inControls[port] = InControlPtr(new FloatInControl(names[port], this));
	
	return true; 		
}

bool ControlExpression::Do()
{
	for (unsigned i = 0; i < _inControls.size(); ++i)
		_mach.stack[i] = _inControls[i]->GetLastValue();

	float r = _mach.execute(_code, _code.begin(), _mach.stack.begin());

	_outControls[0]->SendControl(r);
	
	return true;
}

}

