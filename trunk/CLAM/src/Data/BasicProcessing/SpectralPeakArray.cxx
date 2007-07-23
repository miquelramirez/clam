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

#include "SpectralPeakArray.hxx"
#include "ProcessingDataPlugin.hxx"

namespace CLAM{

namespace Hidden
{
	static ProcessingDataPlugin::Registrator<SpectralPeakArray> dataRegistrator("lightcoral");
}



///////////////////////////////////////////////////////
//
// SpectralPeakArray
//
///////////////////////////////////////////////////////


SpectralPeakArray::SpectralPeakArray(const SpectralPeak& prototype):ProcessingData(eNumAttr)
{
	MandatoryInit();
	InitFromPrototype(prototype);
}

void SpectralPeakArray::DefaultInit()
{
	//Initializing minimum set of attributes (mag, freq and scale)
	AddFreqBuffer();
	AddMagBuffer();
	// MRJ: More forgotten donuts here. What am I missing here?
	// SpectralPeakDetect::CheckOutputType demands all SpectralPeakArrays to comply
	// with the following dyn attribs
	AddBinWidthBuffer();
	AddBinPosBuffer();
	AddPhaseBuffer();
	// MRJ: End of SpectralPeakDetect::CheckOutputType attributes requirements. It
	// seems that someone did an optimization that consisted in not adding these
	// in SpectralPeakDetect::CheckOutputType... Merlijn?
	AddScale();
	AddMinimizeResizes();
	UpdateData();
	SetScale(EScale(EScale::eLinear));
	SetnPeaks(0);
	SetMinimizeResizes(1);
	mIsIndexUpToDate=false;
}

void SpectralPeakArray::InitPeak(SpectralPeak& spectralPeak) const
{
	if (HasMagBuffer())
		spectralPeak.AddMag();
	else spectralPeak.RemoveMag();
	if (HasFreqBuffer())
		spectralPeak.AddFreq();
	else spectralPeak.RemoveFreq();
	if (HasPhaseBuffer())
		spectralPeak.AddPhase();
	else spectralPeak.RemovePhase();
	if (HasBinWidthBuffer())
		spectralPeak.AddBinWidth();
	else spectralPeak.RemoveBinWidth();
	if (HasBinPosBuffer())
		spectralPeak.AddBinPos();
	else spectralPeak.RemoveBinPos();
	spectralPeak.UpdateData();
	spectralPeak.SetScale(GetScale());
}

void SpectralPeakArray::InitFromPrototype(const SpectralPeak& spectralPeak)
{
	if (spectralPeak.HasMag())
		AddMagBuffer();
	else if (HasMagBuffer())
		RemoveMagBuffer();
	if (spectralPeak.HasFreq())
		AddFreqBuffer();
	else if (HasFreqBuffer())
		RemoveFreqBuffer();
	if (spectralPeak.HasPhase())
		AddPhaseBuffer();
	else if (HasPhaseBuffer())
		RemovePhaseBuffer();
	if (spectralPeak.HasBinWidth())
		AddBinWidthBuffer();
	else if (HasBinWidthBuffer())
		RemoveBinWidthBuffer();
	if (spectralPeak.HasBinPos())
		AddBinPosBuffer();
	else if (HasBinPosBuffer())
		RemoveBinPosBuffer();
	UpdateData();
}


void SpectralPeakArray::InitSpectralPeak(SpectralPeak& spectralPeak) const
{
	if (HasMagBuffer()) spectralPeak.AddMag();
	else spectralPeak.RemoveMag();
	if (HasFreqBuffer()) spectralPeak.AddFreq();
	else spectralPeak.RemoveFreq();
	if (HasPhaseBuffer()) spectralPeak.AddPhase();
	else spectralPeak.RemovePhase();
	if (HasBinWidthBuffer()) spectralPeak.AddBinWidth();
	else spectralPeak.RemoveBinWidth();
	if (HasBinPosBuffer()) spectralPeak.AddBinPos();
	else spectralPeak.RemoveBinPos();

	spectralPeak.UpdateData();
}

bool SpectralPeakArray::IsCorrectPrototype(const SpectralPeak& spectralPeak) const
{
	if (HasMagBuffer()!=spectralPeak.HasMag())
		return false;
	if (HasFreqBuffer()!=spectralPeak.HasFreq())
		return false;
	if (HasPhaseBuffer()!=spectralPeak.HasPhase())
		return false;
	if (HasBinWidthBuffer()!=spectralPeak.HasBinWidth())
		return false;
	if (HasBinPosBuffer()!=spectralPeak.HasBinPos())
		return false;
	return true;
}


SpectralPeak SpectralPeakArray::GetThruIndexSpectralPeak(TIndex pos) const
{
	return GetSpectralPeak(GetIndexArray()[pos]);
}

SpectralPeak SpectralPeakArray::GetSpectralPeak(TIndex pos) const
{
	SpectralPeak tmpPeak;
	CLAM_ASSERT(pos<GetnPeaks()&&pos>=0,"SpectralPeakArray::GetSpectralPeak:Out of bounds in peak array");
	InitSpectralPeak(tmpPeak);
	if (HasMagBuffer())
		tmpPeak.SetMag(GetMag(pos));
	if (HasFreqBuffer())
		tmpPeak.SetFreq(GetFreq(pos));
	if (HasPhaseBuffer())
		tmpPeak.SetPhase(GetPhase(pos));
	if (HasBinWidthBuffer())
		tmpPeak.SetBinWidth(GetBinWidth(pos));
	if (HasBinPosBuffer())
		tmpPeak.SetBinPos(GetBinPos(pos));
	tmpPeak.SetScale(GetScale());

	return tmpPeak;
}


void SpectralPeakArray::SetSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,TIndex index)
{
	CLAM_ASSERT(spectralPeak.GetScale()==GetScale(),"SpectralPeakArray::SetSpectralPeak:Incorrect scale in input SpectralPeak");
	CLAM_ASSERT(pos<GetnPeaks(),"SpectralPeakArray::SetSpectralPeak:Out of bounds in peak array");
	CLAM_ASSERT(IsCorrectPrototype(spectralPeak),"SpectralPeakArray::SetSpectralPeak:Incorrect prototype for Spectral Peak");

	if (HasMagBuffer())
		GetMagBuffer()[pos]=spectralPeak.GetMag();
	if (HasFreqBuffer())
		GetFreqBuffer()[pos]=spectralPeak.GetFreq();
	if (HasPhaseBuffer())
		GetPhaseBuffer()[pos]=spectralPeak.GetPhase();
	if (HasBinWidthBuffer())
		GetBinWidthBuffer()[pos]=spectralPeak.GetBinWidth();
	if (HasBinPosBuffer())
		GetBinPosBuffer()[pos]=spectralPeak.GetBinPos();
	if(HasIndexArray())
		GetIndexArray()[pos]=index;
}

void SpectralPeakArray::InsertSpectralPeak(TIndex pos,const SpectralPeak& spectralPeak,bool insertIndex,TIndex index)
{
	CLAM_ASSERT(spectralPeak.GetScale()==GetScale(),"SpectralPeakArray::InsertSpectralPeak:Incorrect scale in input SpectralPeak");
	CLAM_ASSERT(pos<GetnPeaks(),"SpectralPeakArray::InsertSpectralPeak:Out of bounds in peak array");
	CLAM_ASSERT(IsCorrectPrototype(spectralPeak),"SpectralPeakArray::InsertSpectralPeak:Incorrect prototype for Spectral Peak");
	if (HasMagBuffer())
		GetMagBuffer().InsertElem(pos,spectralPeak.GetMag());
	if (HasFreqBuffer())
		GetFreqBuffer().InsertElem(pos,spectralPeak.GetFreq());
	if (HasPhaseBuffer())
		GetPhaseBuffer().InsertElem(pos,spectralPeak.GetPhase());
	if (HasBinWidthBuffer())
		GetBinWidthBuffer().InsertElem(pos,spectralPeak.GetBinWidth());
	if (HasBinPosBuffer())
		GetBinPosBuffer().InsertElem(pos,spectralPeak.GetBinPos());
	if(insertIndex&&HasIndexArray())
		GetIndexArray().InsertElem(pos,index);
}

void SpectralPeakArray::AddSpectralPeak(const SpectralPeak& spectralPeak,bool addIndex,TIndex index)
{
	CLAM_ASSERT(spectralPeak.GetScale()==GetScale(),"SpectralPeakArray::AddSpectralPeak:Incorrect scale in input SpectralPeak");
	CLAM_ASSERT(IsCorrectPrototype(spectralPeak),"SpectralPeakArray::AddSpectralPeak:Incorrect prototype for Spectral Peak");
	if (HasMagBuffer())
		GetMagBuffer().AddElem(spectralPeak.GetMag());
	if (HasFreqBuffer())
		GetFreqBuffer().AddElem(spectralPeak.GetFreq());
	if (HasPhaseBuffer())
		GetPhaseBuffer().AddElem(spectralPeak.GetPhase());
	if (HasBinWidthBuffer())
		GetBinWidthBuffer().AddElem(spectralPeak.GetBinWidth());
	if (HasBinPosBuffer())
		GetBinPosBuffer().AddElem(spectralPeak.GetBinPos());
	if(addIndex&&HasIndexArray())
		GetIndexArray().AddElem(index);
}

void SpectralPeakArray::DeleteSpectralPeak(TIndex pos,bool deleteIndex)
{
	CLAM_ASSERT(GetnPeaks()>0&&pos<GetnPeaks(),"SpectralPeakArray::DeleteSpectralPeak:No spectral peak at that position");
	if (HasMagBuffer())
		GetMagBuffer().DeleteElem(pos);
	if (HasFreqBuffer())
		GetFreqBuffer().DeleteElem(pos);
	if (HasPhaseBuffer())
		GetPhaseBuffer().DeleteElem(pos);
	if (HasBinWidthBuffer())
		GetBinWidthBuffer().DeleteElem(pos);
	if (HasBinPosBuffer())
		GetBinPosBuffer().DeleteElem(pos);
	if(deleteIndex&&HasIndexArray())
		GetIndexArray().DeleteElem(pos);
}

/*Index functionality*/


TIndex SpectralPeakArray::GetPositionFromIndex(TIndex index) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetPositionFromIndex: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetPositionFromIndex: Index table is not up to date");

	//Note: cannot use searching routines because index may not be sorted

	IndexArray& indexArray=GetIndexArray();
	for(int i=0;i<indexArray.Size();i++)
	{
		if(indexArray[i]==index) return	i;//index found
	}
	return -1;//index not found

}

TIndex SpectralPeakArray::GetMaxMagPos() const// returns position of mag maximum
{
	const DataArray & peakMagBuffer=GetMagBuffer();
	// initialize to the first element
	double max = peakMagBuffer[0];
	TIndex maxPos=0;
	for (TIndex i=1;i<GetnPeaks();i++)
	{
		const double & mag = peakMagBuffer[i];
		if (mag>max)
		{
			max = mag;
			maxPos = i;
		}
	}
	return maxPos;

}


TIndex SpectralPeakArray::GetMaxMagIndex() const// returns position of mag maximum
{
	const DataArray & peakMagBuffer=GetMagBuffer();
	// only indexed peaks. It returns the max position

	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetMaxMagPosition: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetMaxMagPosition: IndexTable is not up to date");
	const IndexArray & indexArray = GetIndexArray();
	TIndex maxPos=0;
	double max = peakMagBuffer[indexArray[0]];
	// initialize to the first element
	for (TIndex i=1; i<indexArray.Size(); i++)
	{
		const double & mag = peakMagBuffer[indexArray[i]];
		if (mag>max)
		{
			max = mag;
			maxPos = i;
		}
	}
	return maxPos;

}

void SpectralPeakArray::ResetIndices() // reset all indices
{

	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::ResetIndices: Index array is not instantiated");
	IndexArray& indexArray=GetIndexArray();
	TSize nPeaks=GetnPeaks();
	//Resize will only be done once
	if(indexArray.AllocatedSize()!=GetnMaxPeaks())
		indexArray.Resize(GetnMaxPeaks());
	// set size to the number of Peaks
	indexArray.SetSize(nPeaks);

	indexArray.Reset();
	mIsIndexUpToDate=true;
}

void SpectralPeakArray::InitIndices() // Initialize all indices to -1 and set size
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::InitIndices: Index array is not instantiated");
	int i;
	IndexArray& indexArray=GetIndexArray();
	TSize nPeaks=GetnPeaks();
	TSize nMaxPeaks=GetnMaxPeaks();
	indexArray.Resize(nMaxPeaks);
	indexArray.SetSize(nPeaks);
	for(i=0;i<nPeaks;i++)
	{
		indexArray[i]=-1;
	}
	mIsIndexUpToDate=false;
}

void SpectralPeakArray::SetIndicesTo(TIndex val) // Initialize all indices to -1 without resize
{                                                // is upToDate = true !!!
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetIndicesTo: Index array is not instantiated");
	int i;
	IndexArray& indexArray=GetIndexArray();
	TSize nPeaks=GetnPeaks();
	for(i=0;i<nPeaks;i++)
	{
		indexArray[i]=-1;
	}
	mIsIndexUpToDate=true;
}

/* Delete an element from the index array, but not deleting the Peak information */
void SpectralPeakArray::DeleteIndex (TIndex index)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::DeleteIndex: Index array is not instantiated");
	TIndex pos = GetPositionFromIndex(index);
	if (pos>-1)
		GetIndexArray().DeleteElem(pos);
}

void SpectralPeakArray::AddIndex(TIndex index)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::AddIndex: Index array is not instantiated");
	GetIndexArray().AddElem(index);
}

TIndex SpectralPeakArray::GetFirstNonValidIndexPosition(TIndex beginAt) const
{
	IndexArray& indexArray=GetIndexArray();
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetFirstNonValidIndexPosition: Index array is not instantiated");
	CLAM_ASSERT(beginAt<indexArray.Size()&&beginAt>=0,"SpectralPeakArray::SetThruIndexFreq: Out of bounds in Index Array");
	for(int i=beginAt;i<indexArray.Size();i++)
	{
		if(indexArray[i]==-1) return i;
	}
	return -1;
}

int SpectralPeakArray::GetnIndexedPeaks() const
{
	int i, nIndexedPeaks=0;
	IndexArray& indexArray=GetIndexArray();
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetFirstNonValidIndexPosition: Index array is not instantiated");
	for(i=0;i<indexArray.Size();i++)
	{
		if(indexArray[i]!=-1) nIndexedPeaks++;
	}
	return nIndexedPeaks;
}

/* Getters from index */

double SpectralPeakArray::GetThruIndexFreq(TIndex pos) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetThruIndexFreq: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetThruIndexFreq: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::GetThruIndexFreq: Out of bounds in Index Array");
	return GetFreq(GetIndexArray()[pos]);
}

double SpectralPeakArray::GetThruIndexMag(TIndex pos) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetThruIndexMag: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetThruIndexMag: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::GetThruIndexMag: Out of bounds in Index Array");
	return GetMag(GetIndexArray()[pos]);
}

double SpectralPeakArray::GetThruIndexPhase(TIndex pos) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetThruIndexPhase: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetThruIndexPhase: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::GetThruIndexPhase: Out of bounds in Index Array");
	return GetPhase(GetIndexArray()[pos]);
}

double SpectralPeakArray::GetThruIndexBinPos(TIndex pos) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetThruIndexBinPos: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetThruIndexBinPos: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::GetThruIndexBinPos: Out of bounds in Index Array");
	return GetBinPos(GetIndexArray()[pos]);
}

TSize SpectralPeakArray::GetThruIndexBinWidth(TIndex pos) const
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::GetThruIndexBinWidth: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::GetThruIndexBinWidth: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::GetThruIndexBinWidth: Out of bounds in Index Array");
	return GetBinWidth(GetIndexArray()[pos]);
}

/* Setters from index */
void SpectralPeakArray::SetThruIndexSpectralPeak(TIndex pos,SpectralPeak& peak)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexSpectralPeak: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexSpectralPeak: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexSpectralPeak: Out of bounds in Index Array");
	SetSpectralPeak(GetIndexArray()[pos],peak);
}



void SpectralPeakArray::SetThruIndexFreq(TIndex pos,double freq)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexFreq: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexFreq: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexFreq: Out of bounds in Index Array");
	SetFreq(GetIndexArray()[pos],freq);
}

void SpectralPeakArray::SetThruIndexMag(TIndex pos,double mag)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexMag: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexMag: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexMag: Out of bounds in Index Array");
	SetMag(GetIndexArray()[pos],mag);
}

void SpectralPeakArray::SetThruIndexPhase(TIndex pos,double phase)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexPhase: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexPhase: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexPhase: Out of bounds in Index Array");
	SetPhase(GetIndexArray()[pos],phase);
}

void SpectralPeakArray::SetThruIndexBinPos(TIndex pos,double binPos)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexBinPos: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexBinPos: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexBinPos: Out of bounds in Index Array");
	SetBinPos(GetIndexArray()[pos],binPos);
}

void SpectralPeakArray::SetThruIndexBinWidth(TIndex pos,TSize binWidth)
{
	CLAM_ASSERT(HasIndexArray(),"SpectralPeakArray::SetThruIndexBinWidth: Index array is not instantiated");
	CLAM_ASSERT(mIsIndexUpToDate,"SpectralPeakArray::SetThruIndexBinWidth: IndexTable is not up to date");
	CLAM_ASSERT(pos<GetIndexArray().Size()&&pos>=0,"SpectralPeakArray::SetThruIndexBinWidth: Out of bounds in Index Array");
	SetBinWidth(GetIndexArray()[pos],binWidth);
}

void SpectralPeakArray::TodB()
{
	if(GetScale()==EScale::eLog) return;

	DataArray & mag = GetMagBuffer();
	const int nPeaks=GetnPeaks();
	for (int i=0; i<nPeaks; i++)
	{
		if(mag[i]==0) mag[i]=TData(0.0001);
		mag[i]= CLAM_20log10(mag[i]);
	}
	SetScale(EScale::eLog);
}

void SpectralPeakArray::ToLinear()
{
	if(GetScale()==EScale::eLinear) return;

	DataArray & mag = GetMagBuffer();
	const int nPeaks=GetnPeaks();
	for (int i=0; i<nPeaks; i++)
	{
		if(mag[i]==0.0001) mag[i]=0;
		mag[i]= log2lin(mag[i]);
	}
	SetScale(EScale::eLinear);
}

void SpectralPeakArray::CopyMembers(SpectralPeakArray& sourceSpectralPeakArray)
{
	SetScale( sourceSpectralPeakArray.GetScale() );
	SetMinimizeResizes( sourceSpectralPeakArray.GetMinimizeResizes() );

	SetnMaxPeaks(sourceSpectralPeakArray.GetnMaxPeaks());
	SetnPeaks(sourceSpectralPeakArray.GetnPeaks());
	ResetIndices();

	DataArray& srcFreqBuffer = sourceSpectralPeakArray.GetFreqBuffer();
	DataArray& srcMagBuffer = sourceSpectralPeakArray.GetMagBuffer();
	DataArray& srcPhaseBuffer = sourceSpectralPeakArray.GetPhaseBuffer();
	DataArray& srcBinPosBuffer = sourceSpectralPeakArray.GetBinPosBuffer();
	DataArray& srcBinWidthBuffer = sourceSpectralPeakArray.GetBinWidthBuffer();
	IndexArray& srcIndexArray = sourceSpectralPeakArray.GetIndexArray();

	DataArray& targetFreqBuffer = GetFreqBuffer();
	DataArray& targetMagBuffer = GetMagBuffer();
	DataArray& targetPhaseBuffer = GetPhaseBuffer();
	DataArray& targetBinPosBuffer = GetBinPosBuffer();
	DataArray& targetBinWidthBuffer = GetBinWidthBuffer();
	IndexArray& targetIndexArray = GetIndexArray();

	int numberOfPeaks = sourceSpectralPeakArray.GetnPeaks();
	for (int r=0; r < numberOfPeaks;r++)
	{
		// get frequency , mag and phase
		targetFreqBuffer[r] = srcFreqBuffer[r];
		targetMagBuffer[r] = srcMagBuffer[r];
		targetPhaseBuffer[r] = srcPhaseBuffer[r];
		targetBinPosBuffer[r] = srcBinPosBuffer[r];
		targetBinWidthBuffer[r] = srcBinWidthBuffer[r];
		targetIndexArray[r] = srcIndexArray[r];
	}

}

//xamat: this operator is bound to fail if operands have different attributes, should add checking?
SpectralPeakArray SpectralPeakArray::operator+(const SpectralPeakArray& in)
{
	CLAM_ASSERT(in.HasMagBuffer(), "SpectralPeakArray::operator+: second operand needs to have a Magnitude Buffer");
	CLAM_ASSERT(in.HasFreqBuffer(), "SpectralPeakArray::operator+: second operand needs to have a Frequency Buffer");
	CLAM_ASSERT(in.HasPhaseBuffer(), "SpectralPeakArray::operator+: second operand needs to have a Phase Buffer");
	CLAM_ASSERT(in.HasIndexArray(), "SpectralPeakArray::operator+: second operand needs to have an Index Array");
	CLAM_ASSERT(HasMagBuffer(), "SpectralPeakArray::operator+: first operand needs to have a Magnitude Buffer");
	CLAM_ASSERT(HasFreqBuffer(), "SpectralPeakArray::operator+: first operand needs to have a Frequency Buffer");
	CLAM_ASSERT(HasPhaseBuffer(), "SpectralPeakArray::operator+: first operand needs to have a Phase Buffer");
	CLAM_ASSERT(HasIndexArray(), "SpectralPeakArray::operator+: first operand needs to have an Index Array");


	SpectralPeakArray tmp;
	tmp.AddIndexArray();
	tmp.UpdateData();
	tmp.SetScale(in.GetScale());

	tmp.SetnMaxPeaks(GetnMaxPeaks()+in.GetnMaxPeaks());
	tmp.SetnPeaks(tmp.GetnMaxPeaks());

	int origIndex=0,inIndex=0;
	DataArray& inPeakMagArray = in.GetMagBuffer();
	DataArray& inPeakFreqArray = in.GetFreqBuffer();
	DataArray& inPeakPhaseArray = in.GetPhaseBuffer();
	IndexArray& inPeakIndexArray = in.GetIndexArray();

	DataArray& origPeakMagArray = GetMagBuffer();
	DataArray& origPeakFreqArray = GetFreqBuffer();
	DataArray& origPeakPhaseArray = GetPhaseBuffer();
	IndexArray& origPeakIndexArray = GetIndexArray();

	DataArray& tmpPeakMagArray = tmp.GetMagBuffer();
	DataArray& tmpPeakFreqArray = tmp.GetFreqBuffer();
	DataArray& tmpPeakPhaseArray = tmp.GetPhaseBuffer();
	IndexArray& tmpPeakIndexArray = tmp.GetIndexArray();

	bool finished=false,finishedOrig=false, finishedIn=false;
	TSize origSize,inSize;
	origSize=GetnPeaks();
	inSize=in.GetnPeaks();
	//xamat optimizing
	int nAddedPeaks = 0;
	while(!finished)
	{
		if(origIndex>=origSize-1) finishedOrig=true;
		if(inIndex>=inSize-1) finishedIn=true;
		//add always peak with lower freq. If both are equal, add magnitudes (and take original phase?)
		if(finishedOrig)
		{
			if(!finishedIn)
			{
				//note: we could overload the SetSpectralPeak operator but not passing a SpectralPeaks but the values
				tmpPeakMagArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakFreqArray[nAddedPeaks] = inPeakFreqArray[inIndex];
				tmpPeakPhaseArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakIndexArray[nAddedPeaks] = inPeakIndexArray[inIndex]*2+1;

				nAddedPeaks++;
				inIndex++;
			}
			else finished=true;
		}
		else if(finishedIn)
		{
			if(!finishedOrig)
			{
				tmpPeakMagArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakFreqArray[nAddedPeaks] = origPeakFreqArray[origIndex];
				tmpPeakPhaseArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakIndexArray[nAddedPeaks] = origPeakIndexArray[origIndex]*2;

				nAddedPeaks++;
				origIndex++;
			}
			else finished=true;
		}
		else
		{
			if(origPeakFreqArray[origIndex]<inPeakFreqArray[inIndex])
			{
				tmpPeakMagArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakFreqArray[nAddedPeaks] = origPeakFreqArray[origIndex];
				tmpPeakPhaseArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakIndexArray[nAddedPeaks] = origPeakIndexArray[origIndex]*2;
				nAddedPeaks++;
				origIndex++;
			}
			else if(origPeakFreqArray[origIndex]>inPeakFreqArray[inIndex])
			{
				tmpPeakMagArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakFreqArray[nAddedPeaks] = inPeakFreqArray[inIndex];
				tmpPeakPhaseArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakIndexArray[nAddedPeaks] = inPeakIndexArray[inIndex]*2+1;

				nAddedPeaks++;
				inIndex++;
			}
			else
			{
				tmpPeakMagArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakFreqArray[nAddedPeaks] = origPeakFreqArray[origIndex];
				tmpPeakPhaseArray[nAddedPeaks] = origPeakMagArray[origIndex];
				tmpPeakIndexArray[nAddedPeaks] = origPeakIndexArray[origIndex]*2;

				nAddedPeaks++;
				origIndex++;
				tmpPeakMagArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakFreqArray[nAddedPeaks] = inPeakFreqArray[inIndex];
				tmpPeakPhaseArray[nAddedPeaks] = inPeakMagArray[inIndex];
				tmpPeakIndexArray[nAddedPeaks] = inPeakIndexArray[inIndex]*2+1;

				nAddedPeaks++;
				inIndex++;
			}
		}
	}
	tmp.SetnPeaks(nAddedPeaks);
	return tmp;
}

};//namespace

