#include "Complex.hxx"
#include "CircularShift.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "CircularShift"

namespace MTG {

	/* The  Configuration object has at least to have a name */

	void CircularShiftConfig::Init()
	{
		/* the dynamic type takes care if we add an existing attr .. */

		AddName();
		AddAmount();

		/* All Attributes are added */
		UpdateData();
		SetAmount(0);
	}


	/* Processing  object Method  implementations */

	CircularShift::CircularShift() 
		: mAmount("Amount",this)
	{
		Configure(CircularShiftConfig());
	}

	CircularShift::CircularShift(const CircularShiftConfig &c) 
		: mAmount("Amount",this)
	{
		Configure(c);
	}

	CircularShift::~CircularShift()
	{}


	/* Configure the Processing Object according to the Config object */

	bool CircularShift::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const CircularShiftConfig&>(c);
		mAmount.DoControl(mConfig.GetAmount());
		return true;
	}

	/* Setting Prototypes for faster processing */

		bool CircularShift::SetPrototypes(Spectrum& inputs,const Spectrum& out)
		{
			return false;
		}
  
	bool CircularShift::SetPrototypes()
	{
		return false;
	}

	bool CircularShift::UnsetPrototypes()
	{
		return false;
	}

	/* The supervised Do() function */

	bool  CircularShift::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
	/* The  unsupervised Do() function */

	bool  CircularShift::Do(DataArray& in, DataArray& out)
	{
		int i;
		TData amount = mAmount.GetLastValue();
		int size = in.Size();
		TData* inp = in.GetPtr();
		TData* outp = out.GetPtr();
		TData* tmp;

		if (size != out.Size())
			throw(ErrProcessingObj(CLASS"::Do(): input and output vectors do not match"),this);

		if (amount > 0) {
			int ia = (int)amount;
			tmp = new TData[ia];
			for (i=0;i<ia;i++)
				tmp[i] = inp[size - ia + i];
			for (i=size-ia-1;i>= 0;i--)
				outp[i + ia] = inp[i];
			for (i=0;i< ia;i++)
				outp[i] = tmp[i];
		}
		else {
			int ia = (int)-amount;
			tmp = new TData[ia];
			for (i=0;i<ia;i++)
				tmp[i] = inp[i];
			for (i=0;i< (size - ia);i++)
				outp[i] = inp[i+ia];
			for (i=0;i< ia;i++)
				outp[i+size-ia] = tmp[i];
		}


		delete[] tmp;
		return true;
	}

	bool  CircularShift::Do(Spectrum& in, Spectrum& out)
	{
		if (in.GetType().bMagPhase)
			Do(in.GetMagBuffer(),out.GetMagBuffer());
		else if (in.GetType().bComplex)
			throw(ErrProcessingObj(CLASS"::Do(): only implemented for MagBuf"),this);


		return true;
	}

	bool  CircularShift::Do(Audio& in, Audio& out)
	{
		Do(in.GetFloatBuffer(),out.GetFloatBuffer());
		return true;
	}


}
