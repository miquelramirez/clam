#include "Complex.hxx"
#include "AudioDescriptorGen.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "AudioDescriptorGen"

namespace MTG {

	/* The  Configuration object has at least to have a name */

	void AudioDescriptorGenConfig::Init()
	{
		 /* the dynamic type takes care if we add an existing attr .. */
		AddName();


		/* All Attributes are added */
		UpdateData();

	}


	/* Processing  object Method  implementations */

	AudioDescriptorGen::AudioDescriptorGen()
	{
		Configure(AudioDescriptorGenConfig());
	}

	AudioDescriptorGen::AudioDescriptorGen(const AudioDescriptorGenConfig &c)
	{
		Configure(c);
	}

	AudioDescriptorGen::~AudioDescriptorGen()
	{}


	/* Configure the Processing Object according to the Config object */

	bool AudioDescriptorGen::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const AudioDescriptorGenConfig&>(c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool AudioDescriptorGen::SetPrototypes(Audio& input,const AudioDescriptor& out)
	{
		return false;
	}

	bool AudioDescriptorGen::SetPrototypes()
	{
	return false;
	}

	bool AudioDescriptorGen::UnsetPrototypes()
	{
	return false;
	}

	/* The supervised Do() function */

	bool  AudioDescriptorGen::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}

	/* The  unsupervised Do() function */


	bool  AudioDescriptorGen::Do(Audio& input, AudioDescriptor& c)
	{
		TSize size = input.GetSize();
		TData* data = input.GetFloatBuffer().GetPtr();
		TData* moments = NULL;


		/* Very Basic */

		if (c.GetConfig().GetType().temporalCentroid) {
			c.SetTemporalCentroid(Centroid(data,size));
		}

		if (c.GetConfig().GetType().mean) {
			moments = new TData[4];
			Moment(data,size,moments);
			c.SetMean(moments[0]);
		}

		if(c.GetConfig().GetType().variance) {
			if (!moments) {
				moments = new TData[4];
				Moment(data,size,moments);
			}

			c.SetVariance(Variance(moments));
		}

		if (c.GetConfig().GetType().energy) {
			if (!moments) {
				moments = new TData[4];
				Moment(data,size,moments);
			}
			c.SetEnergy(Energy(moments,size));
		}


		if (moments) delete[] moments;

		if (c.GetConfig().GetType().zeroCrossingRate)
			c.SetZeroCrossingRate(Sum<zerocross>(size-1,data));


		/* Attacktime and LogAttacktime (in samples), very stupid algorithm */

		if (c.GetConfig().GetType().attack || c.GetConfig().GetType().logAttackTime) {
			int i;
			TData max = 0.;
			TSize maxindex;

			for (i=0;i<size;i++)
				if (data[i] > max) {
					max = data[i];
					maxindex = i;
				}

			TSize offset = (int) Sum<offsettime>(size,data,&max);
			if (c.GetConfig().GetType().attack) c.SetAttack(maxindex - offset);
			if (c.GetConfig().GetType().logAttackTime) c.SetLogAttackTime(log(maxindex - offset));
		}



		if (c.GetConfig().GetType().riseTime);

		if (c.GetConfig().GetType().decay);

		if (c.GetConfig().GetType().sustain);

		if (c.GetConfig().GetType().release);


		return true;
	}


}
