#ifndef BackgroundNetwork_hxx
#define BackgroundNetwork_hxx

#include <CLAM/Processing.hxx>


namespace CLAM
{

/**
Plays a network player in a background thread in non real-time
*/
class BackgroundNetwork : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 0, ProcessingConfig );
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		};
	};

	Config _config;

public:
	BackgroundNetwork(const Config& config = Config()) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool Do()
	{
		std::cout << "--- BackgroundNetwork::Do()" << std::endl;
		return true;
	}
	const char* GetClassName() const
	{
		return "BackgroundNetwork";
	}
protected:
	bool ConcreteStart()
	{
		return true;
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		return true;
	}
};

} //namespace

#endif
