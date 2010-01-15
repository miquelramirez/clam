#ifndef BinaryCounter_hxx
#define BinaryCounter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>


class BinaryCounter : public CLAM::Processing
{
	typedef CLAM::OutControl<bool> BoolControl;
	typedef std::vector<BoolControl*> BoolControls;
	BoolControls _outputs;
	unsigned _count;
public:
	BinaryCounter()
		: _count(0)
	{
		for (unsigned i=0; i<8; i++)
		{
			std::ostringstream os;
			os << i;
			_outputs.push_back(new BoolControl(os.str(),this));
		}
	}
	~BinaryCounter()
	{
		for (unsigned i=0; i<_outputs.size(); i++)
			delete _outputs[i];
	}
	const char * GetClassName() const { return "BinaryCounter"; }
	bool Do()
	{
		for (unsigned i=0; i<_outputs.size(); i++)
			_outputs[i]->SendControl((1<<i)&_count);
		_count++;
		return true;
	}
};




#endif//BinaryCounter_hxx
