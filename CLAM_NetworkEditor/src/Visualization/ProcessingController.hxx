
#ifndef _PROCESSINGCONTROLLER_
#define _PROCESSINGCONTROLLER_

#include "ModelController.hxx"
#include "ProcessingModel.hxx"

namespace CLAM
{
	class Processing;
	class ProcessingConfig;
}

namespace CLAMVM
{

class InPortAdapter;
class OutPortAdapter;
class InControlAdapter;
class OutControlAdapter;

class ProcessingController : public ModelController, public ProcessingModel
{
private:
	CLAM::Processing* mObserved;
	CLAM::ProcessingConfig* mConfig;
	std::list<InPortAdapter*> mInPortAdapters;
	std::list<OutPortAdapter*> mOutPortAdapters;
	std::list<InControlAdapter*> mInControlAdapters;
	std::list<OutControlAdapter*> mOutControlAdapters;
	typedef std::list<OutPortAdapter*>::iterator OutPortAdapterIterator;
	typedef std::list<InPortAdapter*>::iterator InPortAdapterIterator;
	typedef std::list<OutControlAdapter*>::iterator OutControlAdapterIterator;
	typedef std::list<InControlAdapter*>::iterator InControlAdapterIterator;

protected:
	void OnUpdateConfigFromGUI( CLAM::ProcessingConfig* );
public:
	ProcessingController();
	virtual ~ProcessingController();
	virtual const char* GetClassName() const
	{
		return "ProcessingController";
	}
	virtual bool Publish();
	virtual bool BindTo( CLAM::Processing& obj );
	virtual bool Update();
};

} // namespace CLAMVM

#endif // __PROCESSINGCONTROLLER__
