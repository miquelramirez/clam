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
#include "AudioDescriptors.hxx"


using namespace CLAM;

CLAM::Flags<11>::tFlagValue AudioDescriptorsFlags::sFlagValues[] = {
	{AudioDescriptorsFlags::eMean, "mean"},
	{AudioDescriptorsFlags::eVariance, "variance"},
	{AudioDescriptorsFlags::eTemporalCentroid, "temporalCentroid"},
	{AudioDescriptorsFlags::eAttack, "attack"},
	{AudioDescriptorsFlags::eDecay, "decay"},
	{AudioDescriptorsFlags::eSustain, "sustain"},
	{AudioDescriptorsFlags::eRelease, "release"},
	{AudioDescriptorsFlags::eLogAttackTime, "logAttackTime"},
	{AudioDescriptorsFlags::eEnergy, "energy"},
	{AudioDescriptorsFlags::eZeroCrossingRate, "zeroCrossingRate"},
	{AudioDescriptorsFlags::eRiseTime, "riseTime"},
	{0,NULL}
};

	

AudioDescriptors::AudioDescriptors(const AudioDescriptorsFlags& flags): ProcessingData(eNumAttr)
{
	MandatoryInit();
	SetFlags(flags);
}

void AudioDescriptors::SetFlags(const AudioDescriptorsFlags& flags)
{
	flags.mean?AddMean():RemoveMean();
	flags.variance?AddVariance():RemoveVariance();
	flags.temporalCentroid?AddTemporalCentroid():RemoveTemporalCentroid();
	flags.attack?AddAttack():RemoveAttack();
	flags.decay?AddDecay():RemoveDecay();
	flags.sustain?AddSustain():RemoveSustain();
	flags.release?AddRelease():RemoveRelease();
	flags.logAttackTime?AddLogAttackTime():RemoveLogAttackTime();
	flags.energy?AddEnergy():RemoveEnergy();
	flags.zeroCrossingRate?AddZeroCrossingRate():RemoveZeroCrossingRate();
	flags.riseTime?AddRiseTime():RemoveRiseTime();
	UpdateData();
}

void AudioDescriptors::GetFlags(AudioDescriptorsFlags& flags) const
{
	flags.mean=HasMean()?true:false;
	flags.variance=HasVariance()?true:false;
	flags.temporalCentroid=HasTemporalCentroid()?true:false;
	flags.attack=HasAttack()?true:false;
	flags.decay=HasDecay()?true:false;
	flags.sustain=HasSustain()?true:false;
	flags.release=HasRelease()?true:false;
	flags.logAttackTime=HasLogAttackTime()?true:false;
	flags.energy=HasEnergy()?true:false;
	flags.zeroCrossingRate=HasZeroCrossingRate()?true:false;
	flags.riseTime=HasRiseTime()?true:false;
}


