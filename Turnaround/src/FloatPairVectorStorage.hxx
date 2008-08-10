#ifndef __FloatPairVectorStorage_hxx__
#define __FloatPairVectorStorage_hxx__

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/InPort.hxx>

class FloatPairVectorStorageConfig : public CLAM::ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FloatPairVectorStorageConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, unsigned long, Frames);
	protected:
		void DefaultInit(void);
};

class FloatPairVectorStorage : public CLAM::Processing
{
	typedef std::vector<CLAM::TData> FloatVector;
	typedef std::vector<std::pair<CLAM::TData, CLAM::TData> > FloatPairVector;
	typedef std::list<FloatPairVector> Storage;
	typedef FloatPairVectorStorageConfig Config;

public:

	FloatPairVectorStorage(const Config& cfg = Config());

	virtual const char* GetClassName() const;
	virtual bool Do();
	virtual ~FloatPairVectorStorage();

	const Storage& Data() const;
	FloatVector FirstData();
	FloatVector SecondData();

protected: // methods

	virtual bool ConcreteConfigure(const CLAM::ProcessingConfig& cfgObject);
	virtual bool ConcreteStart();

protected: // attributes

	Config                        _config;
	CLAM::InPort<FloatPairVector> _in;
	FloatVector                   _firstData;
	FloatVector                   _secondData;
	Storage _data;
};

#endif //__FloatPairVectorStorage_hxx__
