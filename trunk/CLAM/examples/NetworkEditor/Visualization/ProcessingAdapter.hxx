
#ifndef _PROCESSINGADAPTER_
#define _PROCESSINGADAPTER_

#include "ModelAdapter.hxx"
#include "ProcessingModel.hxx"

namespace CLAM
{
	class Processing;
}

namespace CLAMVM
{

class InPortAdapter;
class OutPortAdapter;

class ProcessingAdapter : public ModelAdapter, public ProcessingModel
{
private:
	const CLAM::Processing* mObserved;
	std::list<InPortAdapter*> mInPortAdapters;
	std::list<OutPortAdapter*> mOutPortAdapters;
	typedef std::list<OutPortAdapter*>::iterator OutPortAdapterIterator;
	typedef std::list<InPortAdapter*>::iterator InPortAdapterIterator;
public:
	ProcessingAdapter();
	virtual ~ProcessingAdapter();
	virtual const char* GetClassName() const
	{
		return "ProcessingAdapter";
	}
	virtual bool Publish();
	virtual bool BindTo( const CLAM::Processing& obj );
};

} // namespace CLAMVM

#endif // __PROCESSINGADAPTER__
