#ifndef _PublishedOutControls_hxx_
#define _PublishedOutControls_hxx_

#include <vector>
#include <string>

namespace CLAM
{
//foward declaration:
class OutControl;

class PublishedOutControls
{
	std::vector<OutControl*> mOutControls;
public:
	typedef std::vector<OutControl*>::iterator Iterator;
	typedef std::vector<OutControl*>::const_iterator ConstIterator;

	OutControl& GetByNumber(int index) const;
	OutControl& Get(const std::string & name) const;
	int Size() const;
	void Publish( OutControl *);
	void Clear(){ mOutControls.clear(); }
        Iterator Begin();
	Iterator End();	
	ConstIterator Begin() const;
	ConstIterator End() const;
};


} // namespace CLAM

#endif
