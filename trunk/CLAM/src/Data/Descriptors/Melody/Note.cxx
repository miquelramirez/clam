#include "Note.hxx"

using namespace CLAM;

void Note::DefaultInit() 
{ 
	/* Add default attributes */
	AddFundFreq();
	AddEnergy();
	AddTime();
	UpdateData();
	
	/* Set default values */
	SetFundFreq(0);
	SetEnergy(0);
}	


