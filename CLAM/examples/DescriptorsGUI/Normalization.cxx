#include <math.h>
#include <iostream.h>
#include <Processing.hxx>
#include "Normalization.hxx"
#include "BasicStatistics.hxx"



using namespace CLAM;


void NormalizationConfig::DefaultInit()
{
	AddName();
	AddType();
	AddFrameSize();
	UpdateData();
	SetType(1);
	SetFrameSize(4410);	//0.1s at 44.1k

}


Normalization::Normalization()
{
	Configure(NormalizationConfig());
}

Normalization::Normalization(NormalizationConfig& c)
{
	Configure(c);
}

Normalization::~Normalization() {}


bool Normalization::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const NormalizationConfig&> (c);

	mType=mConfig.GetType();
	mFrameSize=mConfig.GetFrameSize();
	
	return true;
}

bool Normalization::Do(void) 
{
	return false;
}
	
bool Normalization::Do(Audio &in) throw(ErrProcessingObj){

	Audio chunk;
	TData max=0;
	TIndex p=0, m=0;
	DataArray energy;
	TData totEnergy=0;
	TData scalFactor;
	
	do
	{
		in.GetAudioChunk(p, p+mFrameSize, chunk);
		TSize size = chunk.GetSize();
		TData* data = chunk.GetBuffer().GetPtr();
		TData* moments = NULL;
		moments = new TData[4];
		Moment(data,size,moments);

		TData temp;
		temp = Energy(moments,size);
		
		//remove silence
		if ( temp>0.3*mFrameSize/4410 ) //seems to be just above the noise due to 8 bits quantization
		{
			energy.AddElem(temp);
			totEnergy += temp;

			if(max<temp) max=temp;		}

		
		p += mFrameSize;
		m++;
		}	while (p<=in.GetSize()-mFrameSize);

	//normalizes according to the max energy 
	if (mType==1) scalFactor=sqrt(max/mFrameSize);

	//normalizes according to to the average energy
	if (mType==2)
	{
		scalFactor=sqrt(totEnergy/in.GetSize());		
	}

	//normalizes according to to the threshold under which lies percent% of
	//the energy values that are not silence
	if (mType==3)
	{
		//find the threshold under which lies percent% of the energy values
		//that are not silence
				
		int percent=90, i;

		sort(energy, energy.Size());

		i=energy.Size()*percent/100;

		scalFactor=sqrt(energy[i-1]/mFrameSize);

	}

		
		
	DataArray tempBuffer(in.GetSize());
	tempBuffer.SetSize(in.GetSize());
		
	for (int n=0; n<in.GetSize(); n++)
		tempBuffer[n] = in.GetBuffer()[n]/scalFactor;

	in.SetBuffer(tempBuffer);
	


	
	return true;
}


void Normalization::sort(DataArray& list, int size)
{
   int pass;   // Number of pass.
   int i;      // Index variable.

   // Do size - 1 passes.
   for (pass = 1; pass < size; ++pass)
      // On each pass, compare size - pass pairs of elements.
      for (i = 0; i < size - pass; ++i)
         if (list[i] > list[i + 1])
            swap(list[i], list[i + 1]);   // Swap if out of ascending
                                          // order.
}


void Normalization::swap(TData& a, TData& b)
{
   float temp;

   temp = a;
   a = b;
   b = temp;
}


