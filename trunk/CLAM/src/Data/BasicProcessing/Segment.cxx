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

#include "Segment.hxx"



namespace CLAM
{

///////////////////////////////////////////////////////
//
// Segment
//
///////////////////////////////////////////////////////

void Segment::DefaultInit()
{
	pParent=NULL;
	mCurrentFrameIndex=0;
	AddprHoldsData();
	AddBeginTime();
	AddEndTime();
	AddSamplingRate();
	AddChildren();//All segments may have children
	UpdateData();
	DefaultValues();
}

void Segment::DefaultValues()
{
	SetBeginTime(0);
	SetEndTime(0);
	SetSamplingRate(44100);
	SetHoldsData( true );

}
 
void Segment::CopyInit(const Segment& prototype)
{
	pParent=prototype.pParent;
	mFramesSearch=prototype.mFramesSearch;
	mFramesSearch.Set(GetFramesArray());
	mCurrentFrameIndex=prototype.mCurrentFrameIndex;
}


void Segment::CopyDataFromParent()
{
	AddFramesArray();
	AddAudio();
	UpdateData();
	TTime beginTime=GetBeginTime();
	TTime endTime=GetEndTime();
	GetAudio().SetDuration(endTime-beginTime);
	GetpParent()->GetAudio().GetAudioChunk(beginTime,endTime,GetAudio());

	/*TODO: right now frames with center after beginning of segment are added so
	they may include samples outside of range: is this correct?*/
	TIndex position=0;
	if(beginTime>0)//if beginTime=0 it makes no sense searching and Find will return -1
		position=GetpParent()->FindFrame(beginTime);
	TIndex finalPosition=GetpParent()->FindFrame(endTime);
	while(position<=finalPosition)
	{
		AddFrame(GetpParent()->GetFramesArray()[position]);
		position++;
	}
}


int Segment::GetnFrames() const
{
	// Faster method when available
	if(GetHoldsData())
		return GetFramesArray().Size();

	/* TODO: if GetBeginTime()==0 Find in Search Array returns -1!!
	but, will it happen the same with the last one*/
	if (GetBeginTime()==0)
		return FindFrame(GetEndTime());

	return FindFrame(GetEndTime())-FindFrame(GetBeginTime());
}


const Frame& Segment::GetFrame(TIndex pos) const
{
	CLAM_ASSERT(GetHoldsData()||pParent,
		"Segment::GetFrame: No available frames");

	if(GetHoldsData())
		return GetFramesArray()[pos];

	return pParent->GetFrame(pos);

}

Frame& Segment::GetFrame(TIndex pos)
{
	CLAM_ASSERT(GetHoldsData()||pParent,
		"Segment::GetFrame:No available frames");

	if(GetHoldsData())
		return GetFramesArray()[pos];

	return pParent->GetFrame(pos);

}

void Segment::AddFrame(Frame& newFrame)
{
	CLAM_ASSERT(GetHoldsData()||pParent,
		"Segment::AddFrame: No available frame array attribute");

	if(GetHoldsData())
		GetFramesArray().AddElem(newFrame);
	else
		pParent->GetFramesArray().AddElem(newFrame);

	SetEndTime(newFrame.GetCenterTime());

}

void Segment::DeleteFrame(TIndex pos)
{
	CLAM_ASSERT(pos<=GetnFrames(),"Segment::DeleteFrame: Index out of bounds");
	CLAM_ASSERT(GetHoldsData()||pParent,
		"Segment::DeleteFrame: No available frame array attribute");

	if(GetHoldsData())
		GetFramesArray().DeleteElem(pos);
	else
		pParent->GetFramesArray().DeleteElem(pos);

	if (GetnFrames()==0)
	{
		SetEndTime(0);
		SetBeginTime(0);
	}

	else 
	{
		if(pos==GetnFrames())//it was the last frame
			SetEndTime(GetFrame(GetnFrames()-1).GetCenterTime());

		if(pos==0)//it was first frame
			SetBeginTime(GetFrame(0).GetCenterTime());
	}
}

TIndex Segment::FindFrame(TTime time) const
{
	CLAM_ASSERT(GetHoldsData()||pParent,
		"Segment::FindFrame:No available frame array attribute");

	if(!GetHoldsData())
		return pParent->FindFrame(time);
	Frame tmpFrame;
	tmpFrame.SetCenterTime(time);
	if (GetnFrames()<1) return -1;
	return GetFramesSearch().Find(tmpFrame);

}

Frame& Segment::GetFrame(TTime time)
{
	return GetFrame(FindFrame(time));
}

const Frame& Segment::GetFrame(TTime time) const
{
	return GetFrame(FindFrame(time));
}

void Segment::DeleteFrame(TTime time)
{
	DeleteFrame(FindFrame(time));
}

void Segment::SetHoldsData(bool holdsData)
{
	SetprHoldsData(holdsData);
	if(holdsData)
	{
		//Adding necessary attributes
		AddFramesArray();
		AddAudio();
		UpdateData();

		//Initializing some atributes
		GetFramesSearch().Set(GetFramesArray());
		if(pParent!=NULL)
		{
			CopyDataFromParent();
			pParent=NULL;
		}
	}
	else
	{
		RemoveFramesArray();
		RemoveAudio();
		UpdateData();
	}
}

} // namespace CLAM

