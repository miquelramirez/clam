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
	static const unsigned NXEmitter=8;
	static const unsigned NYEmitter=10;
	static const unsigned NZEmitter=1;
	static const unsigned NXReceiver=8;
	static const unsigned NYReceiver=10;
	static const unsigned NZReceiver=1;
	
	typedef std::vector<ComplexSpectrum> ImpulseResponse;
	Config _config;
	OutPort< ImpulseResponse* > _impulseResponse;
	InControl _emitterX;
	InControl _emitterY;
	InControl _receiverX;
	InControl _receiverY;
	ImpulseResponse _responseSpectrums;
	std::vector<ImpulseResponse> _database;

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
		_emitterX.SetBounds(1,NXEmitter);
		_emitterY.SetBounds(1,NYEmitter);
		_receiverX.SetBounds(1,NXReceiver);
		_receiverY.SetBounds(1,NYReceiver);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_database.resize( NXEmitter*NYEmitter*NXReceiver*NYReceiver );
		unsigned i=0;
		for (unsigned xEmitter=1; xEmitter<=NXEmitter; xEmitter++)
		for (unsigned yEmitter=1; yEmitter<=NYEmitter; yEmitter++)
		for (unsigned zEmitter=1; zEmitter<=NZEmitter; zEmitter++)
		for (unsigned xReceiver=1; xReceiver<=NXReceiver; xReceiver++)
		for (unsigned yReceiver=1; yReceiver<=NYReceiver; yReceiver++)
		for (unsigned zReceiver=1; zReceiver<=NZReceiver; zReceiver++)
		{
			std::ostringstream os;
			os << _config.GetPath() << _config.GetPrefix() 
				<< "_emissor_"<< xEmitter << "-" << yEmitter << "-" << zEmitter
				<< "_receptor_" << xReceiver << "-" << yReceiver << "-" << zReceiver
				<< ".wav" << std::flush;
			std::string errorMsg;
			if (!ComputeResponseSpectrums( os.str(), _database[i], _config.GetFrameSize(), errorMsg ))
			{
				AddConfigErrorMessage(errorMsg);
				return false;
			}
			i++;
		}

		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	unsigned getNearestIndex(
		int x1, int y1, int z1, 
		int x2, int y2, int z2)
	{
		return
			z2 + NZReceiver * (
			y2 + NYReceiver * (
			x2 + NXReceiver * (
			z1 + NZEmitter * (
			y1 + NYEmitter * (
			x1
			)))));
	}
	bool Do()
	{
		int x1 = _emitterX.GetLastValue()+0.5;
		int y1 = _emitterY.GetLastValue()+0.5;
		int z1 = 1;
		int x2 = _receiverX.GetLastValue()+0.5;
		int y2 = _receiverY.GetLastValue()+0.5;
		int z2 = 1;
		_impulseResponse.GetData()= &_database[getNearestIndex(x1,y1,z1,x2,y2,z2)];
		_impulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseDatabaseFetcher_hxx

