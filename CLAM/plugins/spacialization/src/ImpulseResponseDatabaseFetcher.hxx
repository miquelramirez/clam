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

#ifndef ImpulseResponseDatabaseFetcher_hxx
#define ImpulseResponseDatabaseFetcher_hxx

#include <CLAM/InControl.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/DirectoryName.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <vector>

namespace CLAM
{



class ImpulseResponseDatabaseFetcher : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, DirectoryName, Path);
		DYN_ATTRIBUTE( 2, public, std::string, Prefix);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFrameSize(512);
		};
	};
private:
	
	Config _config;
	OutPort< ImpulseResponse* > _impulseResponse;
	OutPort< ImpulseResponse* > _previousImpulseResponse;
	InControl _emitterX;
	InControl _emitterY;
	InControl _receiverX;
	InControl _receiverY;
	ImpulseResponseDatabase _database;
	ImpulseResponse * _previous;
	int _numberOfConfigureCalls;

public:
	const char* GetClassName() const { return "ImpulseResponseDatabaseFetcher"; }
	ImpulseResponseDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
		, _previousImpulseResponse("PreviousImpulseResponse", this)
		, _emitterX("emitterX", this)
		, _emitterY("emitterY", this)
		, _receiverX("receiverX", this)
		, _receiverY("receiverY", this)
		, _previous(0)
		, _numberOfConfigureCalls(0)
	{
		Configure( config );
		_emitterX.SetBounds(0,1);
		_emitterY.SetBounds(0,1);
		_receiverX.SetBounds(0,1);
		_receiverY.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		std::cout << "ImpulseResponseDatabaseFetcher::ConcreteConfigure"<<std::endl;
		CopyAsConcreteConfig(_config, config);
		_numberOfConfigureCalls++;
		if (_numberOfConfigureCalls<=2) 
			return false;
		std::string errorMsg;
		if (!_database.loadImpulseResponseDatabase(_config.GetPath() + _config.GetPrefix(), _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
		}
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	static unsigned map(InControl & control, unsigned limit)
	{
		unsigned result = unsigned(std::floor(control.GetLastValue()*limit));
		return result < limit? result : limit-1;
	}
	bool Do()
	{

		unsigned x1 = map(_emitterX, _database.NXEmitter);
		unsigned y1 = map(_emitterY, _database.NYEmitter);
		unsigned z1 = 0;
		unsigned x2 = map(_receiverX, _database.NXReceiver);
		unsigned y2 = map(_receiverY, _database.NYReceiver);
		unsigned z2 = 0;

		ImpulseResponse * current = &_database.get(x1,y1,z1,x2,y2,z2);
		if (x1==x2 && y1==y2 && z1==z2) // don't use the IR corresponding to emmiter and receiver at the same position
		{
			current = _previous ? _previous : current;
		}
		_impulseResponse.GetData()= current;
		_previousImpulseResponse.GetData() = _previous ? _previous : current;
		if (_previous != current) 
			std::cout << "IR : "<<x1<<","<<y1<<","<<z1<<" - "<<x2<<","<<y2<<","<<z2<<std::endl;
		_previous = current;
		_impulseResponse.Produce();
		_previousImpulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseDatabaseFetcher_hxx

