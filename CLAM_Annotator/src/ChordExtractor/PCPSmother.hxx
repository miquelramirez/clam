#ifndef PCPSmother_hxx
#define PCPSmother_hxx
#include <list>
#include <vector>
#include <cmath>
#include <sstream>

namespace Simac
{

class PCPSmother
{
public:
	typedef std::vector<double> PCP;
public:
	PCPSmother(double inertia)
		: _inertia(inertia)
	{
		_output.resize(12);
	}
	~PCPSmother()
	{
	}
	void doIt(const PCP & pcp)
	{
		for (unsigned int i=0; i<pcp.size(); i++)
		{
			_output[i]*=_inertia;
			_output[i]+= (1-_inertia) * pcp[i];
		}
	}
	const PCP & output() const
	{
		return _output;
	}
private:
	PCP _output;
	double _inertia;
};

} // namespace Simac

#endif// PCPSmother_hxx

