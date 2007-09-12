#ifndef ImpulseResponseDatabaseFetcher_hxx
#define ImpulseResponseDatabaseFetcher_hxx

#include <CLAM/InPort.hxx>
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
	static const unsigned NXEmitter=7;
	static const unsigned NYEmitter=8;
	static const unsigned NXReceiver=7;
	static const unsigned NYReceiver=8;
	
	typedef std::vector<ComplexSpectrum> ImpulseResponse;
	Config _config;
	OutPort< ImpulseResponse* > _impulseResponse;
	ImpulseResponse _responseSpectrums;
	std::vector<ImpulseResponse> _database;

public:
	const char* GetClassName() const { return "ImpulseResponseDatabaseFetcher"; }
	ImpulseResponseDatabaseFetcher(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_database.resize( NXEmitter*NYEmitter*NXReceiver*NYReceiver );
		const unsigned zEmitter = 1;
		const unsigned zReceiver = 1;
		for (unsigned xEmitter=1; xEmitter<=NXEmitter; xEmitter++)
		for (unsigned yEmitter=1; yEmitter<=NYEmitter; yEmitter++)
		for (unsigned xReceiver=1; xReceiver<=NXReceiver; xReceiver++)
		for (unsigned yReceiver=1; yReceiver<=NYReceiver; yReceiver++)
		{
			std::ostringstream os;
			os << _config.GetPath() << _config.GetPrefix() 
				<< "_emissor_"<< xEmitter << "-" << yEmitter << "-" << zEmitter
				<< "_receptor_" << xReceiver << "-" << yReceiver << "-" << zReceiver
				<< ".wav" << std::flush;
			std::string errorMsg;
			if (!ComputeResponseSpectrums( os.str(), _responseSpectrums, _config.GetFrameSize(), errorMsg ))
			{
				AddConfigErrorMessage(errorMsg);
				return false;
			}
		}

		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	bool Do()
	{
		_impulseResponse.GetData()= &_responseSpectrums;
		_impulseResponse.Produce();
		return true;
	}
};

} // namespace CLAM


#endif // ImpulseResponseDatabaseFetcher_hxx

