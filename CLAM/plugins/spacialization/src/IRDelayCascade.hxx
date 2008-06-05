/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef IRDelayCascade_hxx
#define IRDelayCascade_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Filename.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>
#include <map>
#include <dirent.h>
#include <fstream>
#include <iomanip>

namespace CLAM
{


class IRDelayCascade : public Processing
{ 
private:
	
	InPort< ImpulseResponse* > _inputIR;
	OutPort< ImpulseResponse* > _currentIR;
	OutPort< ImpulseResponse* > _previousIR;
	ImpulseResponse * _previous;

public:
	const char* GetClassName() const { return "IRDelayCascade"; }
	IRDelayCascade(const Config& config = Config()) 
		: _inputIR("Input IR", this)
		, _currentIR("Current IR", this)
		, _previousIR("Previous IR", this)
		, _previous(0)
	{
		Configure( config );
	}

	bool ConcreteStart()
	{
		_previous = 0;
		return true;
	}

public:
	bool Do()
	{
		ImpulseResponse * current = _inputIR.GetData();
		_currentIR.GetData()= current;
		_previousIR.GetData() = _previous ? _previous : current;
		_previous = current;
		_currentIR.Produce();
		_previousIR.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // IRDelayCascade_hxx

