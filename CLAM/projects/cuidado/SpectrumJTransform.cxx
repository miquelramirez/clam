#include "Complex.hxx"
#include "SpectrumJTransform.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "SpectrumJTransform"

namespace MTG {

  /* The  Configuration object has at least to have a name */

	void SpectrumJTransformConfig::Init()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();


             /* All Attributes are added */
	     UpdateData();

	}


  /* Processing  object Method  implementations */

	SpectrumJTransform::SpectrumJTransform()
	{
		Configure(SpectrumJTransformConfig());
	}

	SpectrumJTransform::SpectrumJTransform(const SpectrumJTransformConfig &c)
	{
		Configure(c);
	}

	SpectrumJTransform::~SpectrumJTransform()
	{}


  /* Configure the Processing Object according to the Config object */

	bool SpectrumJTransform::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
	    mConfig = dynamic_cast<const SpectrumJTransformConfig&>(c);	    
	    return true;
	}

  /* Setting Prototypes for faster processing */

        bool SpectrumJTransform::SetPrototypes(Spectrum& in,const Spectrum& out)
        { return false;
	}
  
        bool SpectrumJTransform::SetPrototypes()
        {
	return false;
	}

        bool SpectrumJTransform::UnsetPrototypes()
        {
	return false;
	}

  /* The supervised Do() function */

        bool  SpectrumJTransform::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
	
  bool SpectrumJTransform::Check(Spectrum& in,Spectrum& out) 
	{
  
		if (!in.HasMagBuffer())
			throw(ErrProcessingObj(CLASS"::Check(): Need a MagBuffer"),this);
		return true;
  }

  /* The  unsupervised Do() function */

	bool  SpectrumJTransform::Do(Spectrum& in, Spectrum& out)
	{

	  int size = in.GetSize();
	  int i;
	  std::cerr << "Spectrumtransform Do() function missing" << std::endl;

	  Check(in,out);

	  for (i=0;i<size;i++)
	    out.GetMagBuffer()[i] = in.GetMagBuffer()[i]*0.01;

	  return true;
	}


}
