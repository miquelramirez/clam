#include "Complex.hxx"
#include "LikelihoodComposite.hxx"
#include "ErrProcessingObj.hxx"
#include "WindowGenerator.hxx"
#include "CircularShift.hxx"
#include "PeakLikelihood.hxx"

#define CLASS "LikelihoodComposite"


namespace MTG {


	/* The  Configuration object has at least to have a name */

	void LikelihoodCompositeConfig::Init()
	{
		/* the dynamic type takes care if we add an existing attr .. */
		
		AddName();
		AddSize();
		AddNumBands();

		/* All Attributes are added */
		UpdateData();
		SetSize(1024);
		SetNumBands(20);
		
	}


	/* Processing  object Method  implementations */

	LikelihoodComposite::LikelihoodComposite()
	{
		AttachChildren();
		Configure(LikelihoodCompositeConfig());
	}

	LikelihoodComposite::LikelihoodComposite(LikelihoodCompositeConfig &c)
	{
		AttachChildren();
		Configure(c);
	}

	LikelihoodComposite::~LikelihoodComposite()
	{}


	void LikelihoodComposite::AttachChildren()
	{
		windowGen.SetParent(this);
		shift.SetParent(this);
		likelihood.SetParent(this);
		prod.SetParent(this);
	}

	void LikelihoodComposite::ConfigureChildren()
	{
		int size = mConfig.GetSize();

		WindowGeneratorConfig winconf;
		winconf.AddType();
		winconf.UpdateData();
		winconf.SetType(EWindowType::eTriangular);
		winconf.SetSize(mConfig.GetSize());
		CircularShiftConfig   shiftconf;
		PeakLikelihoodConfig likeconf;
		SpecProductConfig prodconf;
		SpecTypeFlags sflags;
		sflags.bMagPhase=true;
		sflags.bMagPhaseBPF=false;
		sflags.bComplex=false;

		SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSize(mConfig.GetSize()/2+1);

		winconf.SetNormalize(EWindowNormalize::eNone);
		
		windowGen.Configure(winconf);
		//		shift.Configure(shiftconf);
		likelihood.Configure(likeconf);
		prod.Configure(prodconf);
		window.Configure(sconfig);
		window.SetSize(mConfig.GetSize()/2+1);
	}

	/* Configure the Processing Object according to the Config object */

	bool LikelihoodComposite::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const LikelihoodCompositeConfig&>(c);	    


		ConfigureChildren();
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool LikelihoodComposite::SetPrototypes(Spectrum& inputs,const Spectrum* out[])
	{ 
		return false;
	}

	bool LikelihoodComposite::SetPrototypes()
	{
		return false;
	}

	bool LikelihoodComposite::UnsetPrototypes()
	{
		return false;
	}

	/* The supervised Do() function */

	bool  LikelihoodComposite::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
	
	/* The  unsupervised Do() function */

	bool  LikelihoodComposite::Do(Spectrum& input, Spectrum* out[])
	{
		int i;
		int numBands = mConfig.GetNumBands();
		int bw = 2*input.GetSize()/(numBands+1);
		int start;
		TData* bdef = NULL;

		if (mConfig.HasBandDefinitions())
			bdef = mConfig.GetBandDefinitions();
		
		for (i=0;i<numBands;i++) {
			char name[50];

			if (bdef) {
				start = (int) bdef[2*i];
				bw = (int) bdef[2*i+1];
			}
			else 
				start = i*bw/2;
			/* window with size */
			windowGen.SetSize(bw);
			windowGen.Do(window);

			shift.SetAmount(start);
			shift.Do(window,window);

			prod.Do(input,window,*out[i]);
			likelihood.Do(*out[i],*out[i]);

		}
		return true;
	}


}
