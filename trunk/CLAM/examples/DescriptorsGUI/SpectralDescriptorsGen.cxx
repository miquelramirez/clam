#include "Complex.hxx"
#include "SpectralDescriptorsGen.hxx"
#include "ErrProcessingObj.hxx"
#include "SpectralDescriptors.hxx"
#include "Array.hxx" 
#include "MFCCGen.hxx" 
#define CLASS "SpectralDescriptorsGen"

//#define DEB(x) x
#define DEB(x)

namespace CLAM {

  /* The  Configuration object has at least to have a name */

	void SpectralDescriptorsGenConfig::DefaultInit()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();

             /* All Attributes are added */
	     UpdateData();

	}


  /* Processing  object Method  implementations */

	SpectralDescriptorsGen::SpectralDescriptorsGen()
	{
		Configure(SpectralDescriptorsGenConfig());
	}

	SpectralDescriptorsGen::SpectralDescriptorsGen(const SpectralDescriptorsGenConfig &c)
	{
		Configure(c);
	}

	SpectralDescriptorsGen::~SpectralDescriptorsGen()
	{}


  /* Configure the Processing Object according to the Config object */

	bool SpectralDescriptorsGen::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

  /* Setting Prototypes for faster processing */

        bool SpectralDescriptorsGen::SetPrototypes(Spectrum& input,const SpectralDescriptors& out)
        { return false;
	}
  
        bool SpectralDescriptorsGen::SetPrototypes()
        {
	return false;
	}

        bool SpectralDescriptorsGen::UnsetPrototypes()
        {
	return false;
	}

  /* The supervised Do() function */

        bool  SpectralDescriptorsGen::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  /* The  unsupervised Do() function */


    bool  SpectralDescriptorsGen::Do(Spectrum& input, SpectralDescriptors& c)
	{
	  TSize size = input.GetSize();
	  TData* data = input.GetMagBuffer().GetPtr();
	  TData* moments = NULL;

	  if (c.HasCentroid()) {
	    c.SetCentroid(Centroid(data,size));
	  }

	  if (c.HasMean()) {
	    moments = new TData[4];
	    Moment(data,size,moments);
	    c.SetMean(moments[0]);
	    DEB(cout << " m " << c.GetMean());
	  }

	  if (c.HasGeometricMean()) {
	    moments = new TData[4];
	    Moment(data,size,moments);
	    c.SetGeometricMean(moments[0]);
	    DEB(cout << " gm " << c.GetGeometricMean());
	  }

	  if (c.HasEnergy()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }
	    c.SetEnergy(Energy(moments,size));
	    DEB(cout << " e " << c.GetEnergy());
	  }


	  if(c.HasMoment2()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }	    
	    c.SetMoment2(moments[1]);
	    DEB(cout << " m2 " << c.GetMoment2());
	  }

	  if(c.HasMoment3()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }	    
	    c.SetMoment3(moments[2]);
	    DEB(cout << " m3 " << c.GetMoment3());
	  }

	  if(c.HasMoment4()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }	    
	    c.SetMoment4(moments[3]);
	    DEB(cout << " m4 " << c.GetMoment4());
	  }

	  if(c.HasMoment5()) {
	    /* Not implemented */
	  }

	  if(c.HasMoment6()) {
	    /* Not implemented */
	  }

	  if (c.HasTilt()) {
	    c.SetTilt(SpectralTilt(data,size,1.0,moments));
	    DEB(cout << " t " << c.GetTilt());
	  }

	  if (c.HasKurtosis()) {
	    c.SetKurtosis(Kurtosis(moments));
	    DEB(cout << " k " << c.GetKurtosis());
	  }	  
	  
	 if (c.HasIrregularity())
	    c.SetIrregularity(0);


	 if (c.HasFlatness())
	    c.SetFlatness(SpectralFlatnessMeasure(data , size , moments));


	 if (c.HasStrongPeak())
	    c.SetStrongPeak(0);
	 
	 if (c.HasHFC())
	    c.SetHFC(HighFrequencyCoefficient(data , size));
	
	 if (c.HasMFCC() || c.HasBandEnergy()) 	 
	 { 
		//MFCC & Band Energy calculation
		mMFCCGen.Do(input);
		if(c.HasMFCC())
			c.SetMFCC(mMFCCGen.GetMFCC());
	   
		if (c.HasBandEnergy())
			c.SetBandEnergy(mMFCCGen.GetFilteredData());
	 }
		 
	 
	 if (moments) delete[] moments;

	  return true;
	}


}
