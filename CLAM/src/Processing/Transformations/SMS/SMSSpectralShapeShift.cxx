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

#include "SMSSpectralShapeShift.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
	
namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, SMSSpectralShapeShift> regSMSSpectralShapeShift("SMSSpectralShapeShift");
}

bool SMSSpectralShapeShift::Do(const SpectralPeakArray& 
			inPeaks,SpectralPeakArray& outPeaks)
{
	if (!mConfig.GetPreserveOuts())
	{
		outPeaks = inPeaks; //TODO big cludge for streaming
	}

	mSpectralEnvelope.SetSpectralRange(mSpectralRange);
 	
	if (!mSpectralEnvelopeExtract.Do(inPeaks,mSpectralEnvelope))
		return true; // Not enough peaks for an envelope

	TData amount=mAmount.GetLastValue();

	//Shift spectral shape
	BPF& magBPF=mSpectralEnvelope.GetMagBPF();
	int nPoints=magBPF.Size();
	for(int i=0;i<nPoints;i++)
	{
		magBPF.SetXValue(i, magBPF.GetXValue(i)+amount);
	}
	magBPF.UpdateSplineTable();
	//Apply new spectral shape 
	mSpectralEnvelopeApply.Do(inPeaks,mSpectralEnvelope,outPeaks);
	return true;
}


}

