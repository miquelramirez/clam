#include "Complex.hxx"
#include "SpectrumMovingAverage.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "SpectrumMovingAverage"

namespace MTG {

  /* The  Configuration object has at least to have a name */

	void SpectrumMovingAverageConfig::Init()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();


             /* All Attributes are added */
	     UpdateData();

	}


  /* Processing  object Method  implementations */

	SpectrumMovingAverage::SpectrumMovingAverage()
	{
		Configure(SpectrumMovingAverageConfig());
	}

	SpectrumMovingAverage::SpectrumMovingAverage(const SpectrumMovingAverageConfig &c)
	{
		Configure(c);
	}

	SpectrumMovingAverage::~SpectrumMovingAverage()
	{}


  /* Configure the Processing Object according to the Config object */

	bool SpectrumMovingAverage::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
	    mConfig = dynamic_cast<const SpectrumMovingAverageConfig&>(c);	    
			mWinGenConf = new WindowGeneratorConfig();
			mWinGenConf->SetSize(1024);
			mWinGen = new WindowGenerator(*mWinGenConf);
			

	    return true;
	}

  /* Setting Prototypes for faster processing */

        bool SpectrumMovingAverage::SetPrototypes(Spectrum& inputs,const Spectrum& out)
        { return false;
	}
  
        bool SpectrumMovingAverage::SetPrototypes()
        {
	return false;
	}

        bool SpectrumMovingAverage::UnsetPrototypes()
        {
	return false;
	}

	void SpectrumMovingAverage::Start(void)
	{
		ProcessingObject::Start();
		mWinGen->Start();
	}
	

  /* The supervised Do() function */

        bool  SpectrumMovingAverage::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  TSize SpectrumMovingAverage::Bark(TSize num)
	{
		/* To be implemented */
		return 256;
	} 
	
  bool SpectrumMovingAverage::Check(Spectrum& in,Spectrum& out) 
	{
  
		if (!in.HasMagBuffer())
			throw(ErrProcessingObj(CLASS"::Check(): Need a MagBuffer"),this);
		return true;
  }

  /* The  unsupervised Do() function */

	bool  SpectrumMovingAverage::Do(Spectrum& in, Spectrum& out)
	{

	  std::cerr << "SpectrumMovingAverage::Do() function missing" << std::endl;
	  Check(in,out);
		int i,j;
		int size;
		int numBands =  in.GetSize();

		for (i=0;i<numBands;i++) {
			size = Bark(i);
			TData  avg;

			/* setsize/resize call ;o */
			mArray.SetSize(size);
			mArray.Resize(size);

			/* get the  window */
			mWinGen->SetSize(size);
			mWinGen->Do(mArray);

			TData* inarray = in.GetMagBuffer().GetPtr() + i;
			TData* outarray = out.GetMagBuffer().GetPtr();
			/* do the averaging check this, especially the final division*/

			avg = 0.0;
			for (j=0;j<size && i+j < numBands;j++) 
				avg += inarray[j]*mArray[j];
			avg /= (TData) j;

			outarray[i] = avg;	 
		}
		return true;
	}	
}
