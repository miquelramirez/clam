#include "Complex.hxx"
#include "HarmonicDescriptorGen.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "HarmonicDescriptorGen"

namespace MTG {

	/* The  Configuration object has at least to have a name */

	void HarmonicDescriptorGenConfig::Init()
	{
		/* the dynamic type takes care if we add an existing attr .. */

		AddName();


		/* All Attributes are added */
		UpdateData();

	}


	/* Processing  object Method  implementations */

	HarmonicDescriptorGen::HarmonicDescriptorGen()
	{
		Configure(HarmonicDescriptorGenConfig());
	}

	HarmonicDescriptorGen::HarmonicDescriptorGen(const HarmonicDescriptorGenConfig &c)
	{
		Configure(c);
	}

	HarmonicDescriptorGen::~HarmonicDescriptorGen()
	{}


	/* Configure the Processing Object according to the Config object */

	bool HarmonicDescriptorGen::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const HarmonicDescriptorGenConfig&>(c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool HarmonicDescriptorGen::SetPrototypes(SpectralPeakArray& inputs,const HarmonicDescriptor& out)
	{ 
		return false;
	}

	bool HarmonicDescriptorGen::SetPrototypes()
	{
	return false;
	}

	bool HarmonicDescriptorGen::UnsetPrototypes()
	{
		return false;
	}

	/* The supervised Do() function */

	bool  HarmonicDescriptorGen::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}

	/* The  unsupervised Do() function */

	bool  HarmonicDescriptorGen::Do(SpectralPeakArray& input, HarmonicDescriptor& out)
	{
		return true;
	}


}
