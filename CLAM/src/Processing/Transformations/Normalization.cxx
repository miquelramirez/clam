/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "DataTypes.hxx"
#include "CLAM_Math.hxx"
#include "Normalization.hxx"
#include "Stats.hxx"



using namespace CLAM;


void NormalizationConfig::DefaultInit()
{
	AddType();
	AddFrameSize();
	UpdateData();
	SetType(1);
	SetFrameSize(2205);//0.05s at 44.1k

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


bool Normalization::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig,c);

	mType=mConfig.GetType();
	mFrameSize=mConfig.GetFrameSize();
	
	return true;
}

bool Normalization::Do(void) 
{
	return false;
}
	
bool Normalization::Do(Audio &in){

	Audio chunk;
	TData max=0;
	TIndex p=0, m=0;
	DataArray energy;
	TData totEnergy=0;
	TData scalFactor = 0;

	do
	{
		in.GetAudioChunk(p, p+mFrameSize, chunk);
		/* unused: TSize size = chunk.GetSize(); */
		DataArray moments(4);
		moments.SetSize(4);
		Stats myStats(&chunk.GetBuffer());
		myStats.GetMoments(moments, FifthOrder);

		TData temp = myStats.GetEnergy();

		//remove silence
		if ( temp>0.3*mFrameSize/4410 ) //seems to be just above noise due to 8 bits quantization
		{
			energy.AddElem(temp);
			totEnergy += temp;

			if(max<temp) max=temp;
		}

		p += mFrameSize;
		m++;
	} while (p<=in.GetSize()-mFrameSize);

	//normalizes according to the max energy 
	if (mType==1) scalFactor=sqrt(max/mFrameSize);

	//normalizes according to the average energy
	else if (mType==2)
	{
		scalFactor=sqrt(totEnergy/in.GetSize());		
	}

	//normalizes according to the threshold under which lies percent% of
	//the energy values that are not silence
	else if (mType==3)
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
   TData temp;

   temp = a;
   a = b;
   b = temp;
}


