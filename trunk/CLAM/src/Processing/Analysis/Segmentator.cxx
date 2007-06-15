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

#include "Segmentator.hxx"
#include "Point.hxx"
#include "Segment.hxx"
#include <iostream>
#include "SegmentDescriptors.hxx"


namespace CLAM
{
	class SegmentBoundaries
	{
	public:
		Array < Array < PointTmpl < int , TData > > > mArray;

		SegmentBoundaries(int size):mArray(size)
		{
		}
	};
}

using namespace CLAM;

///////////////////////////////////////////////////////////
///////////////		Segmentator Config		///////////////
///////////////////////////////////////////////////////////		

std::ostream& operator << (std::ostream& myStream, const TDescriptorsParams& a)
{
	switch (a.id)
	{
		case SpectralMeanId:
		{
			myStream << "SpectralMean";
			break;
		}
		case SpectralGeometricMeanId :
		{
			myStream << "SpectralGeometricMean";
			break;
		}
		case SpectralEnergyId:
		{
			myStream << "SpectralEnergy";
			break;
		}
		case SpectralCentroidId :
		{
			myStream << "SpectralCentroid";
			break;
		}
		case SpectralMoment2Id:
		{
			myStream << "SpectralMoment2";
			break;
		}
		case SpectralMoment3Id:
		{
			myStream << "SpectralMoment3";
			break;
		}
		case SpectralMoment4Id:
		{
			myStream << "SpectralMoment4";
			break;
		}
		case SpectralMoment5Id:
		{
			myStream << "SpectralMoment5";
			break;
		}
		case SpectralMoment6Id:
		{
			myStream << "SpectralMoment6";
			break;
		}
		case SpectralFlatnessId :
		{
			myStream << "SpectralFlatness";
			break;
		}
		case SpectralKurtosisId  :
		{
			myStream << "SpectralKurtosis";
			break;
		}
		case FundamentalId  :
		{
			myStream << "Fundamental";
			break;
		}
		default:
		{
			myStream << "UnknownDescriptor";
			break;
		}
	}
	myStream << "threshold=";
	myStream << a.threshold;
	myStream << " ";
	myStream << "percentil=";
	myStream << a.percentil;
	return myStream;
}

std::istream& operator >> (std::istream& myStream, const TDescriptorsParams& a)
{
	CLAM_ASSERT(false, "TDescriptorParams extractor operator is not implemented");
	return myStream;
}


void SegmentatorConfig::DefaultInit()
{
	//AddAll();
	AddDescriptorsParams();
	AddMinSegmentLength();
	UpdateData();
	Array<TDescriptorsParams> tmpArray(0);
	SetDescriptorsParams(tmpArray);
	SearchArray<TDescriptorsParams> tmpSearch(GetDescriptorsParams());
	SetDescriptorsSearch(tmpSearch);
	
	// Default values
	SetMinSegmentLength(0);
};

void SegmentatorConfig::AddDescParams(const TDescriptorsParams& descParams)
{
	TIndex position;
	if(GetDescriptorsParams().Size()==0) 
		GetDescriptorsParams().AddElem(descParams);
	else if ((position=GetDescriptorsSearch().Find(descParams))==-1)
	{
		if(descParams<GetDescriptorsParams()[0])
			GetDescriptorsParams().InsertElem(0,descParams);
		else
			GetDescriptorsParams().AddElem(descParams);
	}
	else
		GetDescriptorsParams().InsertElem(position,descParams);
}

/*false if descriptor is not found, true if it is*/
bool SegmentatorConfig::FindDescParams(TDescriptorsParams& descParams)
{
	int pos;
	if((pos=GetDescriptorsSearch().Find(descParams))!=-1)
	{
		descParams.percentil=GetDescriptorsParams()[pos].percentil;
		descParams.threshold=GetDescriptorsParams()[pos].threshold;
		return true;
	}
	return false;
}

void SegmentatorConfig::ClearDescParams() {
	GetDescriptorsParams().Init();
}

///////////////////////////////////////////////////////////
///////////////			Segmentator			///////////////
///////////////////////////////////////////////////////////

Segmentator::Segmentator()
{
	Configure(SegmentatorConfig());
}

Segmentator::Segmentator(const SegmentatorConfig& c)
{
	Configure(c);
}

Segmentator::~Segmentator()
{
}

bool Segmentator::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	return true;
}

bool Segmentator::Do()
{
	CLAM_DEBUG_ASSERT(IsRunning(), "Segmentator: Do(): Not in execution mode");

	CLAM_ASSERT(false, "Segmentator: Do(): Supervised mode not implemented");

	return false;
}


bool Segmentator::Do(Segment& originalSegment,SegmentDescriptors& descriptors)
{
	int nFrames=originalSegment.GetnFrames();
	Matrix descriptorsValues(mConfig.GetDescriptorsParams().Size(),nFrames);
	UnwrapDescriptors(originalSegment, descriptors,descriptorsValues);
	Algorithm(originalSegment,descriptorsValues);
	return true;
}


void Segmentator::UnwrapDescriptors(const Segment& originalSegment, SegmentDescriptors& descriptors,Matrix& descriptorsValues)
{
	int nFrames=originalSegment.GetnFrames();
	int nDescriptors=mConfig.GetDescriptorsParams().Size();
	for(int i=0;i<nFrames;i++)
	{
/*This looks ugly but right now is the only way to deal with it*/
		int z=0;
		TData value;
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMeanId)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMean();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralGeometricMeanId )
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetGeometricMean();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralEnergyId)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetEnergy();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralCentroidId )
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetCentroid();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMoment2Id)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMoment2();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMoment3Id)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMoment3();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMoment4Id)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMoment4();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMoment5Id)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMoment5();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralMoment6Id)
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMoment6();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralFlatnessId )
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetFlatness();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==SpectralKurtosisId  )
		{
			value=descriptors.GetFrameD(i).GetSpectrumD().GetMagnitudeKurtosis();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==FundamentalId  )
		{
			value=originalSegment.GetFrame(i).GetFundamental().GetFreq();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==AudioEnergyId)
		{
			value=descriptors.GetFrameD(i).GetAudioFrameD().GetEnergy();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==AudioVarianceId)
		{
			value=descriptors.GetFrameD(i).GetAudioFrameD().GetVariance();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==AudioCentroidId)
		{
			value=descriptors.GetFrameD(i).GetAudioFrameD().GetTemporalCentroid();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
		if(z<nDescriptors&&mConfig.GetDescriptorsParams()[z]==AudioZeroCrossingRateId)
		{
			value=descriptors.GetFrameD(i).GetAudioFrameD().GetZeroCrossingRate();
			if(value>mConfig.GetDescriptorsParams()[z].threshold)
				descriptorsValues.SetAt(z,i,value);
			else descriptorsValues.SetAt(z,i,0);
			z++;
		}
	}


}


void Segmentator::Algorithm(Segment& s,const Matrix& values)
{

	// Segmentation objects
	// segment boundaries for each parameter
	int nFrames=s.GetnFrames();
	int nDescriptors=mConfig.GetDescriptorsParams().Size();
	SegmentBoundaries segmentBoundaries(nDescriptors);
	// segment boundaries for each parameter
	segmentBoundaries.mArray.SetSize(nDescriptors);

	for (int z=0;z<nDescriptors;z++)
	{
		segmentBoundaries.mArray[z].AddElem(PointTmpl<int,TData>(0,100));//very high value
	}
	for (int i=0; i<nFrames-4; i++)
	{
		for (int z=0;z<nDescriptors;z++)
		{
			const TData & x3 = values.GetAt(z,i+3);
			const TData & x2 = values.GetAt(z,i+2);
			const TData & x1 = values.GetAt(z,i+1);
			const TData & x0 = values.GetAt(z,i);
			// Avoid div by 0
			if (x2==0) continue;
			
			const TData relevance = fabs((x3-x2)/x2);
			const TData & ratio = mConfig.GetDescriptorsParams()[z].percentil/100;


			if ((x3/x2)>(1+ratio) ||
			    (x3/x2)<(1-ratio))
			{
			/*
			if (i>2)
			{
				if ((x3/x1)>(1+ratio) || (x3/x1)<(1-ratio))
				{
					//if((i-segmentBoundaries.mArray[z][segmentBoundaries.mArray[z].Size()-1])>=mConfig.GetMinSegmentLength())
			*/
				if (( x3>x2 && x2>x1 && x1>x0 )||
				   (  x3<x2 && x2<x1 && x1<x0 ))
				{
					PointTmpl<int,TData>  tmpValue(i+3,relevance/ratio);
					segmentBoundaries.mArray[z].AddElem(tmpValue);
				}
				else if((x3/x2)>(1+2*ratio)||
				        (x3/x2)<(1-2*ratio))
				{
					PointTmpl<int,TData>  tmpValue(i+3,relevance/ratio);
					segmentBoundaries.mArray[z].AddElem(tmpValue);
				}

			}
			/*
				}
			}
			else if (i>2)
			{
				if ((x3/x1)>(1+ratio) || (x3/x1)<(1-ratio))
				{
					//if((i-segmentBoundaries.mArray[z][segmentBoundaries.mArray[z].Size()-1])>=mConfig.GetMinSegmentLength())
					segmentBoundaries.mArray[z].AddElem(i);
				}
			}
			else if (i>3)
			{
				if ((x3/x0)>(1+ratio) || (x3/x0)<(1-ratio))
				{
					//if((i-segmentBoundaries.mArray[z][segmentBoundaries.mArray[z].Size()-1])>=mConfig.GetMinSegmentLength())
					segmentBoundaries.mArray[z].AddElem(i);
				}
			}
			*/
			/*
			if ( x3==0 && x2!=0 )
			{
				Point<int,TData>  tmpValue(i,100);
				segmentBoundaries.mArray[z].AddElem(tmpValue);
			}
			*/

		}
	}
	DataFusion(s,segmentBoundaries);
}

void Segmentator::DataFusion(Segment& s,const SegmentBoundaries& segmentBoundaries)
{

	// DATA FUSION (of the segmentation parameters), taken from Rossignol's Thesis
	// DoNothing,1) Generate probability functions for both parameters
	const int nFrames=s.GetnFrames();
	const int nDescriptors=mConfig.GetDescriptorsParams().Size();
	TData duration=s.GetFrame(0).GetDuration();/*BEWARE!Assuming equal lengthed frames*/
	TData sampleRate=s.GetSamplingRate();

	/*Initializing Probability Matrix*/
	Matrix probabilityMatrix(nFrames,nDescriptors);
	memset(probabilityMatrix.GetBuffer().GetPtr(),0,nFrames*nDescriptors*sizeof(TData));

	/*Setting probability to one wherever a segment boundary was found*/
	for (int z=0;z<nDescriptors;z++)
	{
		for (int n=0;n<segmentBoundaries.mArray[z].Size();n++)
			probabilityMatrix.SetAt(segmentBoundaries.mArray[z][n].GetX(),z,segmentBoundaries.mArray[z][n].GetY());
	}

	// Adding probability values of different descriptors
	Array<TData> globalProb;
	for (int n=0; n<nFrames; n++)
	{
		TData tmpProb=0;
		for(int z=0;z<nDescriptors;z++)
		{
			tmpProb+=probabilityMatrix.GetAt(n,z);
		}
		globalProb.AddElem(tmpProb);
	}

	// MERGE: Two comments, choose one
	// 3) Fusion of too near marks (separated 1 or 2 frames)
	// 3) Fusion of too near marks (separated less than the minSegmentLength)
	// Also compute maximun (to re-use the loop)
	Array<TData> prob_fusion(globalProb);
	{
		int n=0;
		while(globalProb[n]<=0) // Find first frame with prob>0
			n++;
		TData mag=globalProb[n];
		TData gcenter=n*globalProb[n];
		prob_fusion[n]=0;
		for (int m=n+1; m<globalProb.Size(); m++)
		{
			if (globalProb[m]<=0) continue;
			if ((m-n)>mConfig.GetMinSegmentLength())
			{
				// Store information and begin another search
				prob_fusion[(int)(gcenter/mag)]=mag;
				mag=0;
				gcenter=0;
			}
			mag+=globalProb[m];
			gcenter+=m*globalProb[m];
			prob_fusion[m]=0;
			n=m;
		}
	}
	// 4) DELETE SMALL MARKS (1/7 of the max value, parameter that should be optimized...) 
	TData max=0;
	for (int n=0; n<prob_fusion.Size(); n++)
		if (prob_fusion[n]>max)
			max=prob_fusion[n];
	for (int n=0; n<prob_fusion.Size(); n++)
		// MERGE: cuidado max/100 vs. CLAM04 max/7
		if (prob_fusion[n]<=(max/100))
			prob_fusion[n]=0;

	Array<TData> finalSegments; // final segment boundaries in samples
	for (int n=0; n<prob_fusion.Size(); n++)
	{
		if (prob_fusion[n]>0)
			finalSegments.AddElem(n*duration*sampleRate);
	}

	// Store segment boundaries information

	if (finalSegments.Size()<=0) return;

	for (int n=0; n<(finalSegments.Size()-1); n++)
	{
		Segment tmpSegment;
		tmpSegment.SetBeginTime(finalSegments[n]  /sampleRate);
		tmpSegment.SetEndTime  (finalSegments[n+1]/sampleRate);
		tmpSegment.SetpParent(&s);
		tmpSegment.SetHoldsData( false );
		s.GetChildren().AddElem(tmpSegment);
	}

	Segment tmpSegment;
	tmpSegment.SetBeginTime(finalSegments[finalSegments.Size()-1] /sampleRate);
	tmpSegment.SetEndTime(s.GetAudio().GetEndTime());
	tmpSegment.SetpParent(&s);
	s.GetChildren().AddElem(tmpSegment);

}

