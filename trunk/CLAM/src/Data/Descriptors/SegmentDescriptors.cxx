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
#include "Segment.hxx"

using namespace CLAM;



SegmentDescriptors::SegmentDescriptors(Segment* pSegment): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSegment=pSegment;
}

void SegmentDescriptors::DefaultInit() {
	mpSegment=0;
	mpStats=0;
	AddFramesD();
	UpdateData();
}

void SegmentDescriptors::CopyInit(const SegmentDescriptors & copied) {
	mpSegment=copied.mpSegment;
	mpStats=0;
}

const Segment* SegmentDescriptors::GetpSegment() const {
	return mpSegment;
}

void SegmentDescriptors::SetpSegment(Segment* pSegment) {
	mpSegment=pSegment;
	if(mpSegment->HasAudio())
		AddAudioD();
	UpdateData();
	if(mpSegment->HasAudio())
		GetAudioD().SetpAudio(&mpSegment->GetAudio());
	int nFrames=mpSegment->GetnFrames();
	int i;
	for (i=0;i<nFrames;i++){
		GetFramesD()[i].SetpFrame(&(mpSegment->GetFrame(i)));}
}

void SegmentDescriptors::SetFramePrototype(const FrameDescriptors& proto, int nFrames)
{
	int i;
	FrameDescriptors tmpFrD;
	for (i=0;i<nFrames;i++){
		GetFramesD().AddElem(tmpFrD);
		GetFramesD()[i].SetPrototype(proto);}
	mSegmentStats= new StatsTmpl<false,FrameDescriptors,FrameDescriptors>(&GetFramesD());
	if(HasMeanD())
		GetMeanD().SetPrototype(proto);
	if(HasMaxD())
		GetMaxD().SetPrototype(proto);
	if(HasMinD())
		GetMinD().SetPrototype(proto);
	if(HasVarianceD())
		GetVarianceD().SetPrototype(proto);

}

void SegmentDescriptors::Compute()
{
	/*Overriding compute method in base class because right now I don't know
	what to do with member statistics. The best thing would be passing the list of
	FrameDescriptors as processing data but then Descriptors would have to be 
	template and I am not sure that statistics would work, I would need to add
	some operators like + on FrameDescriptors.*/
	ConcreteCompute();
}

void SegmentDescriptors::ConcreteCompute()
{
	int nFrames=mpSegment->GetnFrames();
	int i;
	for (i=0;i<nFrames;i++)
		GetFramesD()[i].Compute();
	if(HasMeanD())
		SetMeanD(mSegmentStats->GetMean());
	if(HasMaxD())
		SetMaxD(mSegmentStats->GetMax());
	if(HasMinD())
		SetMinD(mSegmentStats->GetMin());
	if(HasVarianceD())
		SetVarianceD(mSegmentStats->GetVariance());
}

