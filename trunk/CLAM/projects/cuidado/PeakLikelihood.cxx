#include "Complex.hxx"
#include "PeakLikelihood.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "PeakLikelihood"

namespace MTG {

	/* The	Configuration object has at least to have a name */

	void PeakLikelihoodConfig::Init()
	{
		/* the dynamic type takes care if we add an existing attr .. */

		AddName();


		/* All Attributes are added */
		UpdateData();

	}


	/* Processing  object Method  implementations */

	PeakLikelihood::PeakLikelihood()
	{
		Configure(PeakLikelihoodConfig());
	}

	PeakLikelihood::PeakLikelihood(const PeakLikelihoodConfig &c)
	{
		Configure(c);
	}

	PeakLikelihood::~PeakLikelihood()
	{}


	/* Configure the Processing Object according to the Config object */

	bool PeakLikelihood::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const PeakLikelihoodConfig&>(c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool PeakLikelihood::SetPrototypes(Spectrum& inputs,const Spectrum& out)
	{ return false;
	}
	
	bool PeakLikelihood::SetPrototypes()
	{
	return false;
	}

	bool PeakLikelihood::UnsetPrototypes()
	{
	return false;
	}

	/* The supervised Do() function */

	bool  PeakLikelihood::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
	
	/* The  unsupervised Do() function */

	bool  PeakLikelihood::Do(Spectrum& input, Spectrum& out)
	{
		cerr << CLASS"::Do() function not implemented :)" << endl;
		return true;
	}


}
