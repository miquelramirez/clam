#ifndef _PublishedOutControls_hxx_
#define _PublishedOutControls_hxx_

#include <vector>
#include <string>

namespace CLAM
{
//foward declaration:
class Processing;
class OutControl;

class PublishedOutControls
{
public:
	typedef std::vector<OutControl*>::iterator Iterator;
	PublishedOutControls(Processing* parent);

	OutControl& GetByNumber(int index) const;
	OutControl& Get(std::string name) const;
	int Size() const;
        Iterator Begin();
	Iterator End();

private:
	Processing& mParent;
};


} // namespace CLAM

#endif
