#include "Complex.hxx"
#include "SpectrumClip.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "SpectrumClip"

namespace MTG {

  /* The  Configuration object has at least to have a name */

	void SpectrumClipConfig::Init()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();


             /* All Attributes are added */
	     UpdateData();

	}


  /* Processing  object Method  implementations */

	SpectrumClip::SpectrumClip()
	{
		Configure(SpectrumClipConfig());
	}

	SpectrumClip::SpectrumClip(const SpectrumClipConfig &c)
	{
		Configure(c);
	}

	SpectrumClip::~SpectrumClip()
	{}


  /* Configure the Processing Object according to the Config object */

	bool SpectrumClip::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
	    mConfig = dynamic_cast<const SpectrumClipConfig&>(c);	    
	    return true;
	}

  /* Setting Prototypes for faster processing */

        bool SpectrumClip::SetPrototypes(Spectrum& in1,Spectrum& in2,const Spectrum& out)
        { return false;
	}
  
        bool SpectrumClip::SetPrototypes()
        {
	return false;
	}

        bool SpectrumClip::UnsetPrototypes()
        {
	return false;
	}

  /* The supervised Do() function */

        bool  SpectrumClip::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}

	bool SpectrumClip::Check(Spectrum& in1,Spectrum& in2, const Spectrum& out)
	{
		if (!in1.HasMagBuffer())
			throw(ErrProcessingObj(CLASS"::Check(): Need a MagBuffer"),this);
		if (!in2.HasMagBuffer())
			throw(ErrProcessingObj(CLASS"::Check(): Need a MagBuffer"),this);
		if (!out.HasMagBuffer())
			throw(ErrProcessingObj(CLASS"::Check(): Need a MagBuffer"),this);
		
		return true;
	}

		/* The  unsupervised Do() function */
		
	bool  SpectrumClip::Do(Spectrum& in1,Spectrum& in2, Spectrum& out)
	{
	  int i;
		int size;

		//	  Check(in1,in2,out);

	  TData* buf1 = in1.GetMagBuffer().GetPtr();
	  TData* buf2 = in2.GetMagBuffer().GetPtr();
	  TData* obuf = out.GetMagBuffer().GetPtr();
		size = in1.GetSize();

		for (i=0;i<size;i++)
			obuf[i] = buf1[i] - buf2[i];

	  if (mConfig.HasUpper())
	    {
				TData limit = mConfig.GetUpper();
				for (i=0;i<size;i++)
					if (obuf[i] > limit) obuf[i] = limit;				
	    }

	  if (mConfig.HasLower())
	    {
				TData limit = mConfig.GetLower();
				for (i=0;i<size;i++)
					if (obuf[i] < limit) obuf[i] = limit;				
	    }

	  return true;
	}


}
