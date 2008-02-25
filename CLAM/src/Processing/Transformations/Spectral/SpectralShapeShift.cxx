/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SpectralShapeShift.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralShapeShift",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralShapeShift",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SpectralShapeShift> regSpectralShapeShift("SpectralShapeShift");
	static FactoryRegistrator<ProcessingFactory, SpectralShapeShift> reg = metadata;
}
	
bool SpectralShapeShift::Do(const Spectrum& 
			inSpec,Spectrum& outSpec)
{

	if (!mConfig.GetPreserveOuts())
	{
		outSpec = inSpec; //TODO big cludge for streaming
	}
	
	mLogSpectrum = inSpec;
	mLogSpectrum.ToDB();

	mSpectralEnvelope.SetSpectralRange(mSpectralRange);
 	
	mSpectralPeakDetect.Do(mLogSpectrum, mSpectralPeaks);
	mSpectralEnvelopeExtract.Do(mSpectralPeaks,mSpectralEnvelope);

	TData amount = mSteps.GetLastValue();

	//Shift spectral shape
	BPF& magBPF=mSpectralEnvelope.GetMagBPF();
	int nPoints=magBPF.Size();
	if(nPoints>2)
	{
		magBPF.SetIntpType(EInterpolation::eLinear);
		for(int i=0;i<nPoints;i++)
		{
			magBPF.SetXValue(i, magBPF.GetXValue(i)+amount);
		}
		//magBPF.UpdateSplineTable();
		//Apply new spectral shape 
		outSpec.ToDB();
		mSpectralEnvelopeApply.Do(mLogSpectrum,mSpectralEnvelope,outSpec);
		outSpec.ToLinear();
	}
	return true;
}


}

