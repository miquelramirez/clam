#ifndef Float2IntegerControl_hxx
#define Float2IntegerControl_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <math.h>


class Float2IntegerControl : public CLAM::Processing
{
	CLAM::InControl<float> _input;
	CLAM::OutControl<int> _output;
	unsigned _count;
private:
	bool _lastOutput;
public:
	Float2IntegerControl()
		: _input("Input", this)
		, _output("Output", this)
	{
	}
	~Float2IntegerControl()
	{
	}
	const char * GetClassName() const { return "Float2IntegerControl"; }
	bool Do()
	{
		_output.SendControl(round(_input.GetLastValue()));
		return true;
	}
};




#endif //Float2IntegerControl_hxx
