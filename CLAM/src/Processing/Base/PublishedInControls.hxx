#ifndef _PublishedInControls_hxx_
#define _PublishedInControls_hxx_

#include <vector>
#include <string>

namespace CLAM
{
class InControl;

class PublishedInControls
{
	std::vector<InControl*> mInControls;
public:
	typedef std::vector<InControl*>::iterator Iterator;
	typedef std::vector<InControl*>::const_iterator ConstIterator;

	InControl& GetByNumber(int index) const;
	InControl& Get(const std::string & name) const;
	int Size() const;
	void Publish(InControl *);
	void Clear(){ mInControls.clear(); }
        Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;
};


} // namespace CLAM
#endif
