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




#include "ProcessingData.hxx"
#include "FrameDescriptors.hxx"
#include "Frame.hxx"


using namespace CLAM;

FrameDescriptors::FrameDescriptors(Frame* pFrame): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpFrame=pFrame;
}


void FrameDescriptors::SetpFrame(Frame* pFrame) {
	mpFrame=pFrame;
	//first we add dynamic attributes doing a single UpdateData
	if(mpFrame->HasSpectrum())
		AddSpectrumD();
	if(mpFrame->HasSpectralPeakArray())
		AddSpectralPeakD();
	if(mpFrame->HasResidualSpec())
		AddResidualSpecD();
	if(mpFrame->HasSinusoidalSpec())
		AddSinusoidalSpecD();
	if(mpFrame->HasAudioFrame())
		AddAudioFrameD();
	if(mpFrame->HasSinusoidalAudioFrame())
		AddSinusoidalAudioFrameD();
	if(mpFrame->HasResidualAudioFrame())
		AddResidualAudioFrameD();
	if(mpFrame->HasSynthAudioFrame())
		AddSynthAudioFrameD();
	UpdateData();
	//now we set the data of each descriptor
	if(mpFrame->HasSpectrum())
		GetSpectrumD().SetpSpectrum(&mpFrame->GetSpectrum());
	if(mpFrame->HasSpectralPeakArray())
		GetSpectralPeakD().SetpSpectralPeakArray(&mpFrame->GetSpectralPeakArray());
	if(mpFrame->HasResidualSpec())
		GetResidualSpecD().SetpSpectrum(&mpFrame->GetResidualSpec());
	if(mpFrame->HasSinusoidalSpec())
		GetSinusoidalSpecD().SetpSpectrum(&mpFrame->GetSinusoidalSpec());
	if(mpFrame->HasAudioFrame())
		GetAudioFrameD().SetpAudio(&mpFrame->GetAudioFrame());
	if(mpFrame->HasSinusoidalAudioFrame())
		GetSinusoidalAudioFrameD().SetpAudio(&mpFrame->GetSinusoidalAudioFrame());
	if(mpFrame->HasResidualAudioFrame())
		GetResidualAudioFrameD().SetpAudio(&mpFrame->GetResidualAudioFrame());
	if(mpFrame->HasSynthAudioFrame())
		GetSynthAudioFrameD().SetpAudio(&mpFrame->GetSynthAudioFrame());

}

void FrameDescriptors::Compute()
{
	/*Overriding compute method in base class because right now I don't know
	what to do with member statistics.*/
	ConcreteCompute();
}


void FrameDescriptors::ConcreteCompute()
{
	if(mpFrame->HasSpectrum())
		GetSpectrumD().Compute();
	if(mpFrame->HasSpectralPeakArray())
		GetSpectralPeakD().Compute();
	if(mpFrame->HasResidualSpec())
		GetResidualSpecD().Compute();
	if(mpFrame->HasSinusoidalSpec())
		GetSinusoidalSpecD().Compute();
	if(mpFrame->HasAudioFrame())
		GetAudioFrameD().Compute();
	if(mpFrame->HasSinusoidalAudioFrame())
		GetSinusoidalAudioFrameD().Compute();
	if(mpFrame->HasResidualAudioFrame())
		GetResidualAudioFrameD().Compute();
	if(mpFrame->HasSynthAudioFrame())
		GetSynthAudioFrameD().Compute();

}
