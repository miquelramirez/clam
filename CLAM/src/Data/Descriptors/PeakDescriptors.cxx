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
#include "PeakDescriptors.hxx"


using namespace CLAM;


Flags<11>::tFlagValue PeakDescriptorsFlags::sFlagValues[] = {
	{PeakDescriptorsFlags::eMagnitudeMean, "magnitudeMean"},
	{PeakDescriptorsFlags::eHarmonicCentroid, "harmonicCentroid"},
	{PeakDescriptorsFlags::eSpectralTilt, "spectralTilt"},
	{PeakDescriptorsFlags::eHarmonicDeviation, "harmonicDeviation"},
	{PeakDescriptorsFlags::eFirstTristimulus, "firstTristimulus"},
	{PeakDescriptorsFlags::eSecondTristimulus, "secondTristimulus"},
	{PeakDescriptorsFlags::eThirdTristimulus, "thirdTristimulus"},
	{PeakDescriptorsFlags::eBrightness, "brightness"},
	{PeakDescriptorsFlags::eOddHarmonics, "oddHarmonics"},
	{PeakDescriptorsFlags::eEvenHarmonics, "evenHarmonics"},
	{PeakDescriptorsFlags::eOddToEvenRatio, "oddToEvenRatio"},
	{0,NULL}
};


PeakDescriptors::PeakDescriptors(PeakDescriptorsFlags& flags): ProcessingData(eNumAttr)
{
	MandatoryInit();
	SetFlags(flags);
}




void PeakDescriptors::SetFlags(const PeakDescriptorsFlags & t) 
{

	flags.magnitudeMean?AddMagnitudeMean():RemoveMagnitudeMean();
	flags.harmonicCentroid?AddHarmonicCentroid():AddHarmonicCentroid();
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

void PeakDescriptors::GetFlags(ProcessingDataConfig& flags) const 
{
	flags.magnitudeMean=HasMagnitudeMean();
	flags.harmonicCentroid=HasHarmonicCentroid();
	flags.spectralTilt=HasSpectralTilt();
	flags.harmonicDeviation=HasHarmonicDeviation();
	flags.firstTristimulus=HasFirstTristimulus();
	flags.secondTristimulus=HasSecondTristimulus();
	flags.thirdTristimulus=HasThirdTristimulus();
	flags.brightness=HasBrightness();
	flags.oddHarmonics=HasOddHarmonics();
	flags.evenHarmonics=HasEvenHarmonics();
	flags.oddToEvenRatio=HasOddToEvenRatio();
}

