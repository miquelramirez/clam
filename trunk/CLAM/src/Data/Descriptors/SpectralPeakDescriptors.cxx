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
#include "SpectralPeakDescriptors.hxx"


using namespace CLAM;


Flags<11>::tFlagValue SpectralPeakDescriptorsFlags::sFlagValues[] = {
	{SpectralPeakDescriptorsFlags::eMagnitudeMean, "magnitudeMean"},
	{SpectralPeakDescriptorsFlags::eHarmonicCentroid, "harmonicCentroid"},
	{SpectralPeakDescriptorsFlags::eSpectralTilt, "spectralTilt"},
	{SpectralPeakDescriptorsFlags::eHarmonicDeviation, "harmonicDeviation"},
	{SpectralPeakDescriptorsFlags::eFirstTristimulus, "firstTristimulus"},
	{SpectralPeakDescriptorsFlags::eSecondTristimulus, "secondTristimulus"},
	{SpectralPeakDescriptorsFlags::eThirdTristimulus, "thirdTristimulus"},
	{SpectralPeakDescriptorsFlags::eBrightness, "brightness"},
	{SpectralPeakDescriptorsFlags::eOddHarmonics, "oddHarmonics"},
	{SpectralPeakDescriptorsFlags::eEvenHarmonics, "evenHarmonics"},
	{SpectralPeakDescriptorsFlags::eOddToEvenRatio, "oddToEvenRatio"},
	{0,NULL}
};


SpectralPeakDescriptors::SpectralPeakDescriptors(SpectralPeakDescriptorsFlags& flags): ProcessingData(eNumAttr)
{
	MandatoryInit();
	SetFlags(flags);
}
		

void SpectralPeakDescriptors::SetFlags(const SpectralPeakDescriptorsFlags& flags)
{

	flags.magnitudeMean?AddMagnitudeMean():RemoveMagnitudeMean();
 	flags.harmonicCentroid?AddHarmonicCentroid():RemoveHarmonicCentroid();
	flags.spectralTilt?AddSpectralTilt():RemoveSpectralTilt();
	flags.harmonicDeviation?AddHarmonicDeviation():RemoveHarmonicDeviation();
	flags.firstTristimulus?AddFirstTristimulus():RemoveFirstTristimulus();
	flags.secondTristimulus?AddSecondTristimulus():RemoveSecondTristimulus();
	flags.thirdTristimulus?AddThirdTristimulus():RemoveThirdTristimulus();
	flags.brightness?AddBrightness():RemoveBrightness();
	flags.oddHarmonics?AddOddHarmonics():RemoveOddHarmonics();
	flags.evenHarmonics?AddEvenHarmonics():RemoveEvenHarmonics();
	flags.oddToEvenRatio?AddOddToEvenRatio():RemoveOddToEvenRatio();
	
	UpdateData();
}

void SpectralPeakDescriptors::GetFlags(SpectralPeakDescriptorsFlags& flags) const
{
	flags.magnitudeMean=HasMagnitudeMean()?true:false;
	flags.harmonicCentroid=HasHarmonicCentroid()?true:false;
	flags.spectralTilt=HasSpectralTilt()?true:false;
	flags.harmonicDeviation=HasHarmonicDeviation()?true:false;
	flags.firstTristimulus=HasFirstTristimulus()?true:false;
	flags.secondTristimulus=HasSecondTristimulus()?true:false;
	flags.thirdTristimulus=HasThirdTristimulus()?true:false;
	flags.brightness=HasBrightness()?true:false;
	flags.oddHarmonics=HasOddHarmonics()?true:false;
	flags.evenHarmonics=HasEvenHarmonics()?true:false;
	flags.oddToEvenRatio=HasOddToEvenRatio()?true:false;
}


