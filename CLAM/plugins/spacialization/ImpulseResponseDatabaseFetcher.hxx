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
	InControl _emitterX;
	InControl _emitterY;
	InControl _receiverX;
	InControl _receiverY;
	ImpulseResponse _responseSpectrums;
	ImpulseResponseDatabase _database;

public:
	const char* GetClassName() const { return "ImpulseResponseDatabaseFetcher"; }
	ImpulseResponseDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
		, _emitterX("emitterX", this)
		, _emitterY("emitterY", this)
		, _receiverX("receiverX", this)
		, _receiverY("receiverY", this)
	{
		Configure( config );
		_emitterX.SetBounds(0,1);
		_emitterY.SetBounds(0,1);
		_receiverX.SetBounds(0,1);
		_receiverY.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

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

		_impulseResponse.GetData()= &_database.get(x1,y1,z1,x2,y2,z2);
		_impulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseDatabaseFetcher_hxx

