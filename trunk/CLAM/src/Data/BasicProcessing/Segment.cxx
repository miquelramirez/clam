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

#include "Segment.hxx"



using namespace CLAM;

///////////////////////////////////////////////////////
//
// Segment
//
///////////////////////////////////////////////////////

void Segment::DefaultInit()
{
	pParent=NULL;
	mCurrentFrameIndex=-1;
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
}
 
void Segment::CopyInit(const Segment& prototype)
{
	pParent=prototype.pParent;
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
//Second method could always be used but is much slower
	if(GetHoldsData())
	{
		return GetFramesArray().Size();
	}
	else
	{
		/*TODO: if GetBeginTime()==0 Find in Search Array returns -1!!
		but, will it happen the same with the last one*/
		if (GetBeginTime()==0)
			return FindFrame(GetEndTime());
		else return FindFrame(GetEndTime())-FindFrame(GetBeginTime());
	}
}


const Frame& Segment::GetFrame(TIndex pos) const
{
	if(GetHoldsData())
	{
		return GetFramesArray()[pos];
	}
	else if(pParent!=NULL) 
		return pParent->GetFrame(pos);
	else throw Err("Segment::GetFrame:No available frames");
}

Frame& Segment::GetFrame(TIndex pos)
{
	if(GetHoldsData())
	{
		return GetFramesArray()[pos];
	}
	else if(pParent!=NULL) 
		return pParent->GetFrame(pos);
	else throw Err("Segment::GetFrame:No available frames");
}

void Segment::AddFrame(Frame& newFrame)
{
	if(GetHoldsData())
	{
		GetFramesArray().AddElem(newFrame);
	}
	else if(pParent!=NULL)
		pParent->GetFramesArray().AddElem(newFrame);
	else throw Err("Segment::AddFrame:No available frame array attribute");
	SetEndTime(newFrame.GetCenterTime());
	
}

void Segment::DeleteFrame(TIndex pos)
{
	CLAM_ASSERT(pos<=GetnFrames(),"Segment::DeleteFrame:Index out of bounds")
	if(GetHoldsData())
	{
		GetFramesArray().DeleteElem(pos);
	}
	else if(pParent!=NULL)
		pParent->GetFramesArray().DeleteElem(pos);
	else throw Err("Segment::DeleteFrame:No available frame array attribute");
	if(pos==GetnFrames())//it was the last frame
		SetEndTime(GetFrame(GetnFrames()-1).GetCenterTime());
	if(pos==0)//it was first frame
		SetBeginTime(GetFrame(0).GetCenterTime());

}

TIndex Segment::FindFrame(TTime time) const
{
	if(GetHoldsData())
	{	
		Frame tmpFrame;
		tmpFrame.SetCenterTime(time);
		return GetFramesSearch().Find(tmpFrame);
	}
	else if(pParent!=NULL) 
		return pParent->FindFrame(time);
	else throw Err("Segment::FindFrame:No available frame array attribute");
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
		}
		pParent=NULL;
	}
	else
	{
		RemoveFramesArray();
		RemoveAudio();
		UpdateData();
	}
}
