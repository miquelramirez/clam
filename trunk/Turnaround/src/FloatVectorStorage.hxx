#ifndef __FloatVectorStorage_hxx__
#define __FloatVectorStorage_hxx__

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/InPort.hxx>

class FloatVectorStorage : public CLAM::Processing
{
	typedef std::vector<CLAM::TData> FloatVector;
	
public:

	FloatVectorStorage(const Config& cfg = Config());

	virtual const char* GetClassName() const;
	virtual bool Do();
	virtual ~FloatVectorStorage();
	
	FloatVector Data();

protected: // methods

	virtual bool ConcreteStart();
		
protected: // attributes
	
	Config                    _config;
	CLAM::InPort<FloatVector> _in;
	FloatVector               _data;
};

#endif //__FloatVectorStorage_hxx__
