#include "Complex.hxx"
#include "AudioDescriptorsGen.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "AudioDescriptorsGen"

namespace CLAM {

  /* The  Configuration object has at least to have a name */

	void AudioDescriptorsGenConfig::DefaultInit()
	{
       	     /* the dynamic type takes care if we add an existing attr .. */

	     AddName();

             /* All Attributes are added */
	     UpdateData();

	}


  /* Processing  object Method  implementations */

	AudioDescriptorsGen::AudioDescriptorsGen()
	{
		Configure(AudioDescriptorsGenConfig());
	}

	AudioDescriptorsGen::AudioDescriptorsGen(const AudioDescriptorsGenConfig &c)
	{
		Configure(c);
	}

	AudioDescriptorsGen::~AudioDescriptorsGen()
	{}


  /* Configure the Processing Object according to the Config object */

	bool AudioDescriptorsGen::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
	    return true;
	}

  /* Setting Prototypes for faster processing */

        bool AudioDescriptorsGen::SetPrototypes(Audio& input,const AudioDescriptors& out)
        { return false;
	}
  
        bool AudioDescriptorsGen::SetPrototypes()
        {
	return false;
	}

        bool AudioDescriptorsGen::UnsetPrototypes()
        {
	return false;
	}

  /* The supervised Do() function */

        bool  AudioDescriptorsGen::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  /* The  unsupervised Do() function */


        bool  AudioDescriptorsGen::Do(Audio& input, AudioDescriptors& c)
	{
	  TSize size = input.GetSize();
	  TData* data = input.GetBuffer().GetPtr();
	  TData* moments = NULL;

	  if (c.HasTemporalCentroid()) {
	    c.SetTemporalCentroid(Centroid(data,size));
	  }

	  if (c.HasMean()) {
	    moments = new TData[4];
	    Moment(data,size,moments);
	    c.SetMean(moments[0]);
	  }

	  if(c.HasVariance()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }
	    
	    c.SetVariance(Variance(moments));
	  }

	  if (c.HasEnergy()) {
	    if (!moments) {
	      moments = new TData[4];
	      Moment(data,size,moments);
	    }
	    c.SetEnergy(Energy(moments,size));
	  }


	  if (moments) delete[] moments;

	  if (c.HasZeroCrossingRate())
	    c.SetZeroCrossingRate(Suma<zerocross>(size-1,data));


	  /* Attacktime and LogAttacktime (in samples), very stupid algorithm */

	  if (c.HasAttack() || c.HasLogAttackTime()) {
	    int i;
	    TData max = 0.;
	    TSize maxindex;

	    for (i=0;i<size;i++)
	      if (data[i] > max) {
		max = data[i];
		maxindex = i;
	      }

	    TSize offset = (int) Suma<offsettime>(size,data,&max);
	    if (c.HasAttack()) c.SetAttack(maxindex - offset);
	    if (c.HasLogAttackTime()) c.SetLogAttackTime(log(TData(maxindex - offset)));
	  }



	  if (c.HasRiseTime());

	  if (c.HasDecay());
	  
	  if (c.HasSustain());
	  
	  if (c.HasRelease());
	  

	  return true;
	}


}
