#ifndef BoolControlPrinter_hxx
#define BoolControlPrinter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>


class BoolControlPrinter : public CLAM::Processing
{
	typedef CLAM::TypedInControl<bool> BoolControl;
	typedef std::vector<BoolControl*> BoolControls;
	BoolControls _inputs;
public:
	BoolControlPrinter()
	{
		for (unsigned i=0; i<8; i++)
		{
			std::ostringstream os;
			os << i;
			_inputs.push_back(new BoolControl(os.str(),this));
			_inputs.back()->DoControl(0);
		}
	}
	~BoolControlPrinter()
	{
		for (unsigned i=0; i<_inputs.size(); i++)
			delete _inputs[i];
	}
	const char * GetClassName() const { return "BoolControlPrinter"; }
	bool Do()
	{
		return true;
	}
};




#endif//BoolControlPrinter_hxx
