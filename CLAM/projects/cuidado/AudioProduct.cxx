

#include "AudioProduct.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "AudioProduct"

namespace MTG {

	/* The  Configuration object has at least to have a name */

	void AudioProductConfig::Init()
	{
		/* the dynamic type takes care if we add an existing attr .. */

		AddName();


		/* All Attributes are added */
		UpdateData();

	}


	/* Processing  object Method  implementations */

	AudioProduct::AudioProduct()
	{
		Configure(AudioProductConfig());
	}

	AudioProduct::AudioProduct(const AudioProductConfig &c)
	{
		Configure(c);
	}

	AudioProduct::~AudioProduct()
	{}


	/* Configure the Processing Object according to the Config object */

	bool AudioProduct::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const AudioProductConfig&>(c);
		return true;
	}

	/* Setting Prototypes for faster processing */

	bool AudioProduct::SetPrototypes(Audio& in1,Audio& in2,const Audio& out)
	{ 
		return false;
	}

	bool AudioProduct::SetPrototypes()
	{
		return false;
	}

	bool AudioProduct::UnsetPrototypes()
	{
		return false;
	}

	/* The supervised Do() function */

	bool  AudioProduct::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
	
	void AudioProduct::Check(Audio& in1,Audio& in2, Audio& out)
	{

		if (in1.GetSize() != in2.GetSize() || in1.GetSize() != out.GetSize())
			throw(ErrProcessingObj(CLASS"::Do(): invalid Audio Data Size"),this);

		if (!out.GetType().bFloat)
			throw(ErrProcessingObj(CLASS"::Do(): invalid Audio Data Format"),this);
	}
	

	/* The  unsupervised Do() function */

	bool  AudioProduct::Do(Audio& in1,Audio& in2, Audio& out)
	{
		int size = in1.GetSize();
		int i;

		Check(in1,in2,out);

		TData* inb1 = in1.GetFloatBuffer().GetPtr();
		TData* inb2 = in2.GetFloatBuffer().GetPtr();
		TData* outb = out.GetFloatBuffer().GetPtr();

		for (i=0;i<size;i++) {
			*outb++ = *inb1++ * *inb2++;
		}
		return true;
	}


}
