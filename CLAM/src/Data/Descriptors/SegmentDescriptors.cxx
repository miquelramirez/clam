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
#include "SegmentDescriptors.hxx"


using namespace CLAM;


CLAM::Flags<16>::tFlagValue SegmentDescriptorsFlags::sFlagValues[] = {
	{SegmentDescriptorsFlags::eSpectralMean, "spectralmean"},
	{SegmentDescriptorsFlags::eSpectralGeometricMean, "spectralgeometricMean"},
	{SegmentDescriptorsFlags::eSpectralEnergy, "spectralenergy"},
	{SegmentDescriptorsFlags::eSpectralCentroid, "spectralcentroid"},
	{SegmentDescriptorsFlags::eSpectralMoment2, "spectralmoment2"},
	{SegmentDescriptorsFlags::eSpectralMoment3, "spectralmoment3"},
	{SegmentDescriptorsFlags::eSpectralMoment4, "spectralmoment4"},
	{SegmentDescriptorsFlags::eSpectralMoment5, "spectralmoment5"},
	{SegmentDescriptorsFlags::eSpectralMoment6, "spectralmoment6"},
	{SegmentDescriptorsFlags::eSpectralIrregularity, "spectralirregularity"},
	{SegmentDescriptorsFlags::eSpectralTilt, "spectraltilt"},
	{SegmentDescriptorsFlags::eSpectralFlatness, "spectralflatness"},
	{SegmentDescriptorsFlags::eSpectralKurtosis, "spectralkurtosis"},
	{SegmentDescriptorsFlags::eSpectralStrongPeak, "spectralstrongPeak"},
	{SegmentDescriptorsFlags::eFundamental, "fundamental"},
	{SegmentDescriptorsFlags::eMelody, "melody"},
	{0,NULL}
};

	
SegmentDescriptors::SegmentDescriptors(SegmentDescriptorsFlags& flags): ProcessingData(eNumAttr)
{
	MandatoryInit();
	SetFlags(flags);
}


void SegmentDescriptors::SetFlags(const SegmentDescriptorsFlags& flags)
{
	flags.spectralMean?AddSpectralMean():RemoveSpectralMean();
	flags.spectralGeometricMean?AddSpectralGeometricMean():RemoveSpectralGeometricMean();
	flags.spectralEnergy?AddSpectralEnergy():RemoveSpectralEnergy();
	flags.spectralCentroid?AddSpectralCentroid():RemoveSpectralCentroid();
	flags.spectralMoment2?AddSpectralMoment2():RemoveSpectralMoment2();
	flags.spectralMoment3?AddSpectralMoment3():RemoveSpectralMoment3();
	flags.spectralMoment4?AddSpectralMoment4():RemoveSpectralMoment4();
	flags.spectralMoment5?AddSpectralMoment5():RemoveSpectralMoment5();
	flags.spectralMoment6?AddSpectralMoment6():RemoveSpectralMoment6();
	flags.spectralIrregularity?AddSpectralIrregularity():RemoveSpectralIrregularity();
	flags.spectralTilt?AddSpectralTilt():RemoveSpectralTilt();
	flags.spectralFlatness?AddSpectralFlatness():RemoveSpectralFlatness();
	flags.spectralKurtosis?AddSpectralKurtosis():RemoveSpectralKurtosis();
	flags.spectralStrongPeak?AddSpectralStrongPeak():RemoveSpectralStrongPeak();
	flags.fundamental?AddFundamental():RemoveFundamental();
	flags.melody?AddMelody():RemoveMelody();
	UpdateData();
}


void SegmentDescriptors::GetFlags(SegmentDescriptorsFlags& flags) const
{
	flags.spectralMean=HasSpectralMean()?true:false;
	flags.spectralGeometricMean=HasSpectralGeometricMean()?true:false;
	flags.spectralEnergy=HasSpectralEnergy()?true:false;
	flags.spectralCentroid=HasSpectralCentroid()?true:false;
	flags.spectralMoment2=HasSpectralMoment2()?true:false;
	flags.spectralMoment3=HasSpectralMoment3()?true:false;
	flags.spectralMoment4=HasSpectralMoment4()?true:false;
	flags.spectralMoment5=HasSpectralMoment5()?true:false;
	flags.spectralMoment6=HasSpectralMoment6()?true:false;
	flags.spectralIrregularity=HasSpectralIrregularity()?true:false;
	flags.spectralTilt=HasSpectralTilt()?true:false;
	flags.spectralFlatness=HasSpectralFlatness()?true:false;
	flags.spectralKurtosis=HasSpectralKurtosis()?true:false;
	flags.spectralStrongPeak=HasSpectralStrongPeak()?true:false;
	flags.fundamental=HasFundamental()?true:false;
	flags.melody=HasMelody()?true:false;
}




