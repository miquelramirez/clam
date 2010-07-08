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

