#include "OnsetDetection.hxx"

//Includes for debug display
//#include "CUIDADO_Snapshots.hxx"
#include <fstream>

using namespace CLAM;
using namespace std;



/////////////////////////////////////////////////////////////////////////////////
/* The Configuration object has at least to have a name */	
void OnsetDetectionConfig::DefaultInit()
{
	AddFrameSize();
	AddnBands();
	AddSmoothFiltSize();
	AddBandThreshold();
	AddMinPeakDist();
	AddGlobalThreshold();
	AddDifSize();
	UpdateData();
	/* Set default values */
	SetFrameSize(256);
	SetnBands(20);
	SetSmoothFiltSize(10);
	SetBandThreshold(0.15f); //default value??????
	SetMinPeakDist(10);
	SetGlobalThreshold(120); //default value??????
	SetDifSize(10);			 //no average by default
}


OnsetDetection::OnsetDetection()
{
	AttachChildren();
	Configure(OnsetDetectionConfig());
}

OnsetDetection::OnsetDetection(OnsetDetectionConfig& c)
{
	AttachChildren();
	Configure(c);
}

bool OnsetDetection::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mFrameSize = mConfig.GetFrameSize();
	mFFTSize = mFrameSize * 2 ;
	mnBands = mConfig.GetnBands();
	mBandThreshold = mConfig.GetBandThreshold();
	mMinPeakDist = mConfig.GetMinPeakDist();
	mGlobalThreshold = mConfig.GetGlobalThreshold();
	mDifSize = mConfig.GetDifSize();
		
	//Calculates the smoothing filter coefficient
	mWinSize = mConfig.GetSmoothFiltSize();
	mWinCoef.Resize(mWinSize);
	mWinCoef.SetSize(mWinSize);
	for(int i=0; i<mWinSize; i++)	mWinCoef[i]=0.5+0.5*cos(2*M_PI*i/(mWinSize*2));

	ConfigureData();

	ConfigureChildren();
	
	return true;
}


bool OnsetDetection::ConfigureChildren()
{
	//
	//  Processing Objects Configuration

	////////////////////////////////////////////////////////////////
	// FFT
	FFTConfig fcfg;
	fcfg.SetAudioSize(mFFTSize);
	mFFT.Configure(fcfg);
	mFFT.SetPrototypes (mAudioFrameZeroed, mSpec);

	
	return true;

}

void OnsetDetection::ConfigureData()
{
	/* Audio */
		  
	mAudioFrame.SetSize(mFrameSize);
	mAudioFrame.UpdateData();
	mAudioFrameZeroed.SetSize(mFFTSize);
	mAudioFrameZeroed.UpdateData();
	  	
	
	/* Spectrum */

	SpectrumConfig scfg;
	scfg.SetSize(mFFTSize/2+1);
	mSpec.Configure(scfg);
	mSpec.UpdateData();

}

void OnsetDetection::AttachChildren()
{
	mFFT.SetParent(this);
}



//////////////////////////
//SUPERVISED DO FUNCTION//
//////////////////////////
bool OnsetDetection::Do(Segment &originalSegment)
{
	mDescriptors.Resize(mnBands);
	mDescriptors.SetSize(mnBands);
	DescriptorsCalculation( originalSegment.GetAudio() );
	mnFrames = mDescriptors[0].Size();
	
	Algorithm( originalSegment , mDescriptors);
	
	return true;
}


///////////////////////////
//DESCRIPTORS CALCULATION//
///////////////////////////
bool OnsetDetection::DescriptorsCalculation(Audio& in)
{
	int k=0,i=0;
	int step=mConfig.GetFrameSize();
	int fileSize = in.GetSize();
	
	int percentilCounter=0,lastPercentilCounter=0;
	do
	{      
	  AnalyseFrame(in,i); //compute the segment's frames' descriptors
	    i++;
	    k+=step;

		percentilCounter=(int)((float)k/fileSize*100);
		if(percentilCounter>lastPercentilCounter){
		std::cout << percentilCounter;
		std::cout << "% done \n";
		lastPercentilCounter=percentilCounter;}

	}  while(k<=fileSize-step);
 
			
	cout << "	...Done\n\n";

  return true;
}

//bool DescriptorsCalculation::AnalyseFrame(Segment& in,int frameIndex)
bool OnsetDetection::AnalyseFrame(Audio& in, int frameIndex)
{
  bool ret = true;
  int step=mConfig.GetFrameSize();
  TData samplingRate=in.GetSampleRate();
  TTime centerTime=(frameIndex*step+step/2)/samplingRate;
	
  mFrame.SetDuration(step/samplingRate);  
  mFrame.SetCenterTime(centerTime);
     
  in.GetAudioChunk(frameIndex*step,frameIndex*step+step,mAudioFrame);
 
  ///////////////
  //ZEROPADDING//
  ///////////////
  DataArray tempAudio(mFFTSize);
  tempAudio.SetSize(mFFTSize);
  Audio mFrameZeroed;
  for (int j=0 ; j<tempAudio.Size() ; j++)
	{
		if (j<mFFTSize/4)
			tempAudio[j] = 0;
		if ( j>=mFFTSize/4 && j<(mFFTSize*3/4) )
			tempAudio[j] = mAudioFrame.GetBuffer()[j-mFFTSize/4];
		if (j>=(mFFTSize*3/4) && j<tempAudio.Size())
			tempAudio[j] = 0;
	}
  mAudioFrameZeroed.SetBuffer(tempAudio);

  //FFT
  mFFT.Do(mAudioFrameZeroed, mSpec);	
  
  //Band Energy Calculation
  mMFCCGen.Do(mSpec);
  
  //Sums up bands by pairs
  for (int z=0 ; z<mMFCCGen.GetFilteredData().Size() ; z++)
  {
	mDescriptors[z/2].AddElem(mMFCCGen.GetFilteredData()[z] + mMFCCGen.GetFilteredData()[z+1]);
	z++;
  }			

  /*for (int z=0 ; z<mMFCCGen.GetFilteredData().Size() ; z++)
 	mDescriptors[z].AddElem(mMFCCGen.GetFilteredData()[z]);
	*/			

  return ret;	
}



////////////////////////////////////////
////////SEGMENTATION ALGORITHM//////////
////////////////////////////////////////
void OnsetDetection::Algorithm( Segment& s , Array< Array<TData> >& bandEnergyArray )
{
	int z,i,j;
	
	mEnergyThreshold = 1;
	mOffsetPercentil = 0.4f;

	TData mSampleRate = s.GetSamplingRate();

	Array< Array<TData> > bandSmoothedEnergy(mnBands);	//Smoothed Band Energy Array
	bandSmoothedEnergy.SetSize(mnBands);

	Array< Array<TData> > bandOnsetDetectData(mnBands);	//Data on which detection is performed 
	bandOnsetDetectData.SetSize(mnBands);

	Array< Array<TData> > bandOnsetPos(mnBands);
	bandOnsetPos.SetSize(mnBands);
	Array< Array<TData> > bandOnsetIntensity(mnBands);
	bandOnsetIntensity.SetSize(mnBands);
	TimeIndex tempOnset;
	Array< Array<TimeIndex> > bandOnsets(mnBands);
	bandOnsets.SetSize(mnBands);
	

	DataArray onsetsArray(mnFrames);		//sum of cleaned onsets detected in each bands (Data)
	onsetsArray.SetSize(mnFrames);	
	
	Array<TimeIndex> onsets;				//sum of cleaned onsets detected in each bands (TimeIndex)

	Array<TimeIndex> finalOnsets;			//onsets after final thresholding
	

	//Array for the display

	//Array< Array<TData> > dude(mnBands);
	//dude.SetSize(mnBands);

	
	for ( z=0 ; z<mnBands ; z++)  
	{

		///////////
		//SMOOTHING
		Smoothing( bandEnergyArray[z] , bandSmoothedEnergy[z] );
		cout << "Smoothing done...\n\n";

		
		///////////////////////////////
		//BAND ONSET POSITION DETECTION

		//Detection Function Calculation
		DetectPosition( bandSmoothedEnergy[z] , bandOnsetDetectData[z]);
		cout << "Detection Function Calculated...\n\n";
		
		//PeakPicking for Onset Position Detection
		PeakPick( bandOnsetDetectData[z] , mBandThreshold , bandOnsetPos[z] );
		cout << "Peaks Picked...\n\n";
		

		//////////////////////////////////
		//BAND ONSET INTENSITY EVALUTATION
		DetectIntensity( bandOnsetPos[z] , bandSmoothedEnergy[z] , bandOnsetIntensity[z] );
		cout << "Intensity detected...\n\n";


		//Forms Band Onset TimeIndex Array (Position & Weight)
		for ( j=0 ; j<bandOnsetPos[z].Size() ; j++) {
			tempOnset.SetPosition(bandOnsetPos[z][j]);
			tempOnset.SetWeight(bandOnsetIntensity[z][j]);
			bandOnsets[z].AddElem(tempOnset);
		}
	
		//Store onsets weights before weak onsets deletion		
		/*dude[z].Resize(mnFrames);
		dude[z].SetSize(mnFrames);

		for (i=0;i<bandOnsets[z].Size();i++)
				dude[z][bandOnsets[z][i].GetPosition()]=bandOnsets[z][i].GetWeight();*/
			
		

		

		/////////////////////
		//WEAK ONSET DELETION
		DeleteWeakOnsets( bandOnsets[z] , "del" );

		
		//////////////////////
		//SUMS UP BAND RESULTS
		for ( j=0 ; j<bandOnsets[z].Size() ; j++)
			onsetsArray[bandOnsets[z][j].GetPosition()] += bandOnsets[z][j].GetWeight();

		
		//////////////////////
		//Debuging display

		//CLAMGUI::ShowSnapshot(s.GetAudio() , bandEnergyArray[z] , bandSmoothedEnergy[z] , "Energy & Smoothed Energy for each Band");
		//CLAMGUI::ShowSnapshot( s.GetAudio() , bandSmoothedEnergy[z] , bandOnsetDetectData[z] , "Smoothed Energy & Detection Function, for each Band");
		//CLAMGUI::ShowSnapshot( s.GetAudio() , bandSmoothedEnergy[z] , dude[z] , "Detection Function & Intensity for each onset"); 
		//CLAMGUI::ShowSnapshot( s.GetAudio() , dude, "Onsets detected, for each Band");
	
	}		

	cout << "Integrating Result from each Band...\n\n";

	
	//Forms ordered array of onsets
	for ( i=0 ; i<mnFrames ; i++)
	{
		if (onsetsArray[i] != 0)
		{
		tempOnset.SetPosition(i);
		tempOnset.SetWeight(onsetsArray[i]);
		onsets.AddElem(tempOnset);
		}
	}
	
	
	/////////////////////
	//WEAK ONSET DELETION
	DeleteWeakOnsets( onsets , "sum" );

	DataArray cleanOnsetsArray(mnFrames);
	cleanOnsetsArray.SetSize(mnFrames);	

	for ( j=0 ; j<onsets.Size() ; j++)
			cleanOnsetsArray[onsets[j].GetPosition()] += onsets[j].GetWeight();


	//display
	//CLAMGUI::ShowSnapshot( s.GetAudio() , onsetsArray , cleanOnsetsArray, "Onsets Detected");
	

	////////////////////
	//FINAL THRESHOLDING
	Array<int> boundaries;
	for (j=0 ; j<onsets.Size() ; j++)
	{
		if (onsets[j].GetWeight()>mGlobalThreshold)
		{
			tempOnset.SetPosition(onsets[j].GetPosition());
			tempOnset.SetWeight(onsets[j].GetWeight());
			finalOnsets.AddElem(tempOnset);
			boundaries.AddElem(onsets[j].GetPosition());
		}
	}
	
	/////////////////////////////////////////////////
	// Stores segment boundaries (onsets) information	  
	if(finalOnsets.Size()>0)
	{
		for(j=0; j<(finalOnsets.Size()-1); j++) 
		{
			Segment tmpSegment;
		    tmpSegment.SetBeginTime(finalOnsets[j].GetPosition() * mFrameSize  / mSampleRate);
		    tmpSegment.SetEndTime  (finalOnsets[j+1].GetPosition() * mFrameSize / mSampleRate);
			tmpSegment.SetpParent(&s);
		    s.GetChildren().AddElem(tmpSegment);
		}
		Segment tmpSegment;
		tmpSegment.SetHoldsData(false);
		tmpSegment.SetBeginTime(finalOnsets[finalOnsets.Size()-1].GetPosition() * mFrameSize /mSampleRate);
		tmpSegment.SetEndTime(s.GetEndTime());
		tmpSegment.SetpParent(&s);
		s.GetChildren().AddElem(tmpSegment);
	}

	
	
	///////////////////////////////
	//POST_PROCESSING using PITCH//
	///////////////////////////////
	
	cout <<"Performing Post-Processing";
	
	Array<int> removedBound;
	CheckPitch(s , finalOnsets, removedBound);

	cout <<"...done!\n\n";

	//DEBUG DISPLAY
	DataArray on(mnFrames);
	on.SetSize(mnFrames);
	for (j=0;j<finalOnsets.Size();j++)
	{
		on[finalOnsets[j].GetPosition()]=finalOnsets[j].GetWeight();
	}
	

	DataArray fund;

	TData pitchThresh = 0.1;
	
	for(i=0;i<s.GetnFrames();i++) 
	{
		if (i>0 && i<s.GetnFrames()-1 && fabsf(s.GetFrame(i+1).GetFundamental().GetFreq()-s.GetFrame(i-1).GetFundamental().GetFreq())/s.GetFrame(i+1).GetFundamental().GetFreq()<pitchThresh && fabsf(s.GetFrame(i-1).GetFundamental().GetFreq()-s.GetFrame(i).GetFundamental().GetFreq())/fund[i-1]>pitchThresh && fabsf(s.GetFrame(i+1).GetFundamental().GetFreq()-s.GetFrame(i).GetFundamental().GetFreq())/s.GetFrame(i+1).GetFundamental().GetFreq()>pitchThresh)
			fund.AddElem((s.GetFrame(i+1).GetFundamental().GetFreq()+s.GetFrame(i-1).GetFundamental().GetFreq())/2);
		else
			fund.AddElem(s.GetFrame(i).GetFundamental().GetFreq());
	}

	//CLAMGUI::ShowSnapshot(fund , "pitch");
	Array<TIndex> dispBoundaries;
	if(s.GetChildren().Size()>0)
	{
		for( i=0;i<s.GetChildren().Size();i++)
		{
			dispBoundaries.AddElem((int)(s.GetChildren()[i].GetBeginTime() * 44100.0));
		}
	}
	//CLAMGUI::ShowSnapshot(s.GetAudio() , dispBoundaries , removedBound , "new bounds");


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
	//DEBBUGGING
	/*ofstream store("debug.txt", ios::out);
	for ( z=0 ; z<mnBands ; z++) 
	{
		for (i=0;i<bandOnsetPos[z].Size();i++)
			store << bandOnsetPos[z][i] << "	";
		store << "\n";
	}
	store << "\n\n";
	for ( z=0 ; z<mnBands ; z++) 
	{
		for (i=0;i<bandOnsetIntensity[z].Size();i++)
			store << bandOnsetIntensity[z][i] << "	";
		store << "\n";
	}
	store << "\n\n";
	for (i=0;i<onsets.Size();i++)
		store << onsets[i] << "	";
	store << "\n";
*/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

}


////////////////////////////////
///////////SMOOTHING////////////
////////////////////////////////
void OnsetDetection::Smoothing(DataArray energy, DataArray &smoothedEnergy )
{
	int i, j, k;

	//Rescaling factor
	TData sum=0;
	for(k=0; k<mWinCoef.Size(); k++)
		sum += mWinCoef [k];

	//convolution
	for(i=0; i<mnFrames;i++)
	{	
		TData temp=0;
		for(j=0; j<mWinSize; j++)
		{
			if(i-mWinSize+1+j>=0) 
			{
				temp+=energy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
			}
		}

		if ( (temp/sum) >mEnergyThreshold)
			{smoothedEnergy.AddElem(temp/sum);}
		else 
			{smoothedEnergy.AddElem(mEnergyThreshold);}
	}
}


////////////////////////////////
//////ONSET TIME DETECTION//////
////////////////////////////////
void OnsetDetection::DetectPosition(DataArray in, DataArray &ret)
{
	int i;

	for (i=0 ; i<mnFrames ; i++) 
	{
		if (i==0 || i==mnFrames-1)	//Sets first and last values to zero
			{ret.AddElem(0);}
		else
			{ret.AddElem(log(in[i+1])-log(in[i-1]));}
	}
}



///////////////////////////////////
///////AUTOMATIC THRESHOLDING//////
///////////////////////////////////
//Uses 3 sigma algorithm  [DONOHO 94]:
//		Take n% of the smallest samples
//		Set threshold at 3 * sigma, the standard deviation of these samples.


void OnsetDetection::AutoThresh(DataArray in, int retPerCent, float &ret)
{
	int nNoRetained = in.Size()-retPerCent*in.Size()/100 , maxCount=0 , maxPos , j , k;
	float maxAux = 0;
	DataArray auxIn;/*(retPerCent*in.GetNumColumns()/100), auxIn(in.Size());
	aux.SetSize(in.Size();
	auxIn.SetSize(in.Size());
	*/
	auxIn = in;
		
	
	//Extract the n% smallest values
	for ( j=0 ; j<nNoRetained ; j++ ) {
		for ( k=0 ; k<auxIn.Size() ; k++ ) {
			if (auxIn[k]>maxAux) {
	 			maxAux = auxIn[k];
				maxPos = k;
			}
		}
		auxIn.DeleteElem( maxPos );
		maxAux = 0;
	}

	//Calculates the standard deviation
	double mean=0 , stdDev , var=0;
	for ( j=0 ; j<auxIn.Size() ; j++ )
		mean = mean + auxIn[j];

	mean = mean / auxIn.Size();

	for ( j=0 ; j<=auxIn.Size()-1 ; j++ )
		var = var + pow( auxIn[j]-mean , 2 );
		
	var = var / auxIn.Size();
	
	stdDev = pow( var , 0.5 );
		
	//Threshold
	ret = 3*stdDev;
}



////////////////////////////////
/////////PEAK PICKING///////////
////////////////////////////////
//only return position of peaks
void OnsetDetection::PeakPick(DataArray in, TData threshold , DataArray &ret)
{	
	int i;
	
	for ( i=1 ; i<in.Size()-1 ; i++ ) {
		if (i<2 || i>=in.Size()-2) {
			if (in[i]>threshold && in[i]>in[i-1] && in[i]>in[i+1]) 
				ret.AddElem(i);
		}
		else if (in[i]>threshold && in[i]>in[i-1] && in[i]>in[i+1] && in[i-1]>=in[i-2] && in[i+1]>=in[i+2] )
			ret.AddElem(i);
	}
}



////////////////////////////////////
//BAND ONSET INTENSITY EVALUTATION//
////////////////////////////////////
void OnsetDetection::DetectIntensity(DataArray positionArray , DataArray weightArray , DataArray &ret )
{
	TData tempDeriv , max, sumM, sumP;
	int j , k , pos;
	TSize average;
	for (j=0 ; j<positionArray.Size() ; j++)
	{
		average=mDifSize;
		k = positionArray[j];
		max = 0;

		
		//intensity= max derivative between the onset position and the first negative derivative
		do 
		{

			if (k<mDifSize) average=k;
			if (weightArray.Size()-k-1<mDifSize) average=weightArray.Size()-k-1;

			sumM=sumP=0;

			for (int i=1; i<=average;i++)
			{
				sumM+=log(weightArray[k-i])/average;
				sumP+=log(weightArray[k+i])/average;
			}

			tempDeriv=sumP-sumM;


			if (max<tempDeriv) 
			{
				max = tempDeriv;
				pos = k;
			}
			k++;
		} while (tempDeriv>=0 && k<weightArray.Size()-1);

		if(max<0) max=0;		

		ret.AddElem(max);

	}
}




//////////////////////////////////////////
/////SMALL INTENSITY ONSETS DELETION//////
//PROCESSING OF TOO-CLOSELY SPACED PEAKS//
//////////////////////////////////////////
void OnsetDetection::DeleteWeakOnsets(Array<TimeIndex> &in , char* type)
{
		ofstream store("dude.txt" , ios::out);
		TimeIndex newPeak;
		int j=0 , k=0;
		while ( j < in.Size()-1 ) {
			k=j;
		
			//finds too-closely spaced peaks
			int numMemPeaks=0;
			if ( type == "del" ) {
				while ( k<in.Size()-1  &&  (in[k+1].GetPosition() - in[k].GetPosition()) < mMinPeakDist ) {
					numMemPeaks++;
					k++;
				}
			}
			
			if (type == "sum") {
				while ( k<in.Size()-1  &&  (in[k+1].GetPosition() - in[j].GetPosition()) < mMinPeakDist ) {
					numMemPeaks++;
					k++;
				}
			}

			if ( numMemPeaks!=0 ) {
				//stores too-closely spaced peaks
				Array<TimeIndex> memMat(numMemPeaks+1);
				memMat.SetSize(numMemPeaks+1);			
	
				int m=0, l;
				for (l=j ; l<=k ; l++) {
					memMat[m].SetPosition(in[l].GetPosition());
					memMat[m].SetWeight(in[l].GetWeight() );
					m++;
				}
				
				if ( type == "sum" )
					//Sums the peaks
					PeakSummation( memMat , newPeak );
				
				if ( type == "del" ) 
					//Deletes the peaks
					PeakDeletion( memMat , newPeak );
							
			
				//Update the peak matrix
				for (l=k ; l>j-1 ; l--) {
					if (l==k) { 
						in[l].SetPosition(newPeak.GetPosition());
						in[l].SetWeight(newPeak.GetWeight());
					}
					else in.DeleteElem(l);
					
				}
				for (int u=0 ; u<in.Size() ; u++)
				{
					store << in[u].GetPosition() << "	";
				}
				store << "\n\n";
			}
			else j++;	
		}	
		
		for (int u=0 ; u<in.Size() ; u++)
			{
				store << in[u].GetPosition() << "	";
			}
		
		store << "\n\n\n\n";
}

//PEAK DELETION
void OnsetDetection::PeakDeletion( Array<TimeIndex> in , TimeIndex &ret )
{
	TData max=0;
	int pos;

	for ( int j=0 ; j<in.Size() ; j++) {
		if (in[j].GetWeight()>=max) {
			max=in[j].GetWeight();
			pos=in[j].GetPosition();
		}
	}
	ret.SetPosition(pos);
	ret.SetWeight(max);
}


//PEAK SUMMATION
void OnsetDetection::PeakSummation( Array<TimeIndex> in , TimeIndex &ret )
{
	TData max=0 , sum=0;
	int pos;

	for ( int j=0 ; j<in.Size() ; j++) {
		sum+=in[j].GetWeight();
		if (in[j].GetWeight()>=max) {
			max=in[j].GetWeight();
			pos=in[j].GetPosition();
		}
	}

	ret.SetPosition(pos);
	ret.SetWeight(sum);
}


///////////////////
//POST_PROCESSING//
///////////////////
void OnsetDetection::CheckPitch(Segment& s , Array<TimeIndex> onsets , Array<int>& ret)
{
	int j , i;
	int nAvge = 4 , tempnAvge;
	TData frame , sumP , sumM;
	DataArray fund;

	TData fundChangeThresh = 0.03f;
	TData pitchThresh = 0.1f;
	
	for(i=0;i<s.GetnFrames();i++) 
	{
		if (i>0 && i<s.GetnFrames()-1 && fabsf(s.GetFrame(i+1).GetFundamental().GetFreq()-s.GetFrame(i-1).GetFundamental().GetFreq())/s.GetFrame(i+1).GetFundamental().GetFreq()<pitchThresh && fabsf(s.GetFrame(i-1).GetFundamental().GetFreq()-s.GetFrame(i).GetFundamental().GetFreq())/fund[i-1]>pitchThresh && fabsf(s.GetFrame(i+1).GetFundamental().GetFreq()-s.GetFrame(i).GetFundamental().GetFreq())/s.GetFrame(i+1).GetFundamental().GetFreq()>pitchThresh)
			fund.AddElem((s.GetFrame(i+1).GetFundamental().GetFreq()+s.GetFrame(i-1).GetFundamental().GetFreq())/2);
		else
			fund.AddElem(s.GetFrame(i).GetFundamental().GetFreq());
	}

	j=1;
	do 
	{
		frame = (int)(s.GetChildren()[j].GetBeginTime() * 44100 / 1024);
		sumP=0;
		sumM=0;
		tempnAvge=nAvge;
		if (frame<nAvge) tempnAvge=frame;
		if (s.GetnFrames()-frame-1<nAvge) tempnAvge=s.GetnFrames()-frame-1;

		for(i=1;i<=tempnAvge;i++)
		{
			sumP += log(fund[frame+i]) / tempnAvge;
			sumM += log(fund[frame-i]) / tempnAvge;
		}
		
		if(fabsf(sumP-sumM) > fundChangeThresh || onsets[j].GetWeight()>3*mGlobalThreshold)
			j++;
		else
		{
			ret.AddElem(s.GetChildren()[j].GetBeginTime() * 44100);
			s.GetChildren().DeleteElem(j);
			onsets.DeleteElem(j);
		}
	} while(j<s.GetChildren().Size());
}
