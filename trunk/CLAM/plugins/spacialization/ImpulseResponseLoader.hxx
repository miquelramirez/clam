#ifndef ImpulseResponseLoader_hxx
#define ImpulseResponseLoader_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include "LoadImpulseResponse.hxx"
#include <CLAM/AudioInFilename.hxx>

namespace CLAM
{

class ImpulseResponseLoader : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, FrameSize);
		DYN_ATTRIBUTE( 1, public, AudioInFilename, ImpulseResponse);

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
	OutPort< std::vector<ComplexSpectrum>* > _impulseResponse;
	std::vector<ComplexSpectrum> _responseSpectrums;

public:
	const char* GetClassName() const { return "ImpulseResponseLoader"; }
	ImpulseResponseLoader(const Config& config = Config()) 
		: _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		std::string errorMsg;
		if (!ComputeResponseSpectrums( _config.GetImpulseResponse(), _responseSpectrums, _config.GetFrameSize(), errorMsg ))
		{
			AddConfigErrorMessage(errorMsg);
			return false;
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

#endif // ImpulseResponseLoader_hxx

