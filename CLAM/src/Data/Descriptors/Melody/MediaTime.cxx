#include "MediaTime.hxx"

namespace CLAM {
	void MediaTime::DefaultInit() 
	{ 	
		// Default parameters
		AddBegin();
		AddEnd(); 
		UpdateData();
	}

}; // namespace CLAM
