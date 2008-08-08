#ifndef __FloatVectorStorage_hxx__
#define __FloatVectorStorage_hxx__

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/InPort.hxx>

class FloatVectorStorageConfig : public CLAM::ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FloatVectorStorageConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, unsigned, Bins);
		DYN_ATTRIBUTE (1, public, unsigned long, Frames);
	protected:
		void DefaultInit(void);
};

class FloatVectorStorage : public CLAM::Processing
{
	typedef std::vector<CLAM::TData> FloatVector;
	typedef FloatVectorStorageConfig Config;
	
public:

	FloatVectorStorage(const Config& cfg = Config());

	virtual const char* GetClassName() const;
	virtual bool Do();
	virtual ~FloatVectorStorage();
	
	FloatVector Data();

protected: // methods

	virtual bool ConcreteConfigure(const CLAM::ProcessingConfig& cfgObject);
	virtual bool ConcreteStart();
		
protected: // attributes
	
	Config                    _config;
	CLAM::InPort<FloatVector> _in;
	FloatVector               _data;
};

#endif //__FloatVectorStorage_hxx__
