#ifndef _PublishedInControls_hxx_
#define _PublishedInControls_hxx_

#include <vector>
#include <string>

namespace CLAM
{
//foward declaration:
class Processing;
class InControl;

class PublishedInControls
{
public:
	typedef std::vector<InControl*>::iterator Iterator;
	PublishedInControls(Processing* parent);

	InControl& GetByNumber(int index) const;
	InControl& Get(std::string name) const;
	int Size() const;
        Iterator Begin();
	Iterator End();

private:
	Processing& mParent;
};


} // namespace CLAM
#endif
