#include "XMLStorage.hxx"
#include "DPFundamental.hxx"

int main()
{
	CLAM::Segment seg;
	// load data from XML file 
	CLAM::XMLStorage::Restore(seg,"../../data/fundfreq_data.xml");

	CLAM::VM::PlotFundamental(seg,"Fundamental"); 

	return 0;
}


// END
