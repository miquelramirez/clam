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


#include "Segmentator.hxx"

#include "AudioFileIn.hxx"
#include "FFT_rfftw.hxx"
#include "PeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include <iostream>
#include <math.h>

#include "Segment.hxx"
#include "Frame.hxx"
#include "SegmentAnalysisSystem.hxx"
 
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include "Vector.hxx"

#include <fstream>
#ifdef CLAM_USE_XML
#	include "XMLStorage.hxx"
#	include "XMLStaticAdapter.hxx"
#	include "XMLComponentAdapter.hxx"
#	include "XMLable.hxx"
#endif//CLAM_USE_XML

#include "Builder.hxx"


using namespace CLAM;


void Compute();
void DisplaySegments(Segment& root,TData samplingRate);

int main()
{

	try{
		Compute();
	}
	catch(Err error)
		{
			error.Print();
			std::cerr << "Abnormal Program Termination" << std::endl;
		}
	catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	
	std::clog << "Finished successfully!";
	return 0;
}

void Compute()
{
		
	std::string fileName;
	std::cout<<"Enter File Name: \n";
	std::cin>>fileName;
	// Display flags
	bool displayAudio = false;
	bool displaySegments=true;
	
	//GUI objects
	Builder*		GUIBuilder = CLAM::Builder::GetInstance("FLTK");
	GUIObject* myGUI = GUIBuilder->GetGUI();

	TData analysisFrameSize=1024;		
	 
	//The File I/O PO
	AudioFileIn myAudioFileIn;
	AudioFileConfig infilecfg;
	infilecfg.SetName("FileIn");
	infilecfg.SetFilename(fileName);
	infilecfg.SetFiletype(EAudioFileType::eWave);
	myAudioFileIn.Configure(infilecfg);
	
			
	/////////////////////////////////////////////////////////////////////////////
	// Initialization of the processing data objects :
	TSize fileSize=myAudioFileIn.Size();
	Audio readAudio; // read Audio object 
	readAudio.SetSize(fileSize);
	TData samplingRate=readAudio.GetSampleRate();
	
	
	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(readAudio);


	if (displayAudio)
		myGUI->ShowSnapshot(readAudio);


	// The system that contains all the analysis processing objects:
	SegmentAnalysisSystemConfig globalConfig;
	globalConfig.SetAudioFrameSize(analysisFrameSize);
	globalConfig.SetSamplingRate(samplingRate);
	SegmentAnalysisSnapshotsFlags tmpFlags;
	//intra frame display-flags !!!!!
	tmpFlags.bShowAudioFrame=0;
	tmpFlags.bShowFrameSpectrum=0;
	globalConfig.SetDisplayFlags(tmpFlags);
	SegmentAnalysisSystem s(globalConfig);
	
	// Spectral Segment that will actually hold data
	SegmentConfig segmentCfg;
	segmentCfg.SetHoldsData(true);
	float duration=fileSize/samplingRate;
	std::cout << "The audio file has a total of" << duration << std::endl;
	segmentCfg.SetEndTime(duration);
	Segment mySegment(segmentCfg);
	mySegment.SetAudio(readAudio);
	 
	
	  
	/////////////////////////////////////////////////////////////////////////////
	// The main processing loop.
	int k=0,i=0;
	int step=analysisFrameSize;
	  
	
	std::cout << "Analysis Processing loop began, 0%% done \n";
	s.Start();
	int percentilCounter=0,lastPercentilCounter=0;
	do
	{      
		s.Do(mySegment,i);
		  i++;
		  k+=step;
		percentilCounter=(int)((float)k/fileSize*100);
		if(percentilCounter>lastPercentilCounter){
		std::cout << percentilCounter;
		std::cout << "% done \n";
		lastPercentilCounter=percentilCounter;}
	}  while(k<=fileSize-step);

	std::cout << "Processing loop finished, starting Segmentation \n";
	
	//Configuring Segmentator to work only with fundamental and energy
	SegmentatorConfig sgConfig;
	TDescriptorsParams tmpParams;
	tmpParams.id=SpectralEnergyId;
	tmpParams.percentil=40;
	tmpParams.threshold=0.0016;
	sgConfig.AddDescParams(tmpParams);
	tmpParams.id=FundamentalId;
	tmpParams.percentil=4;
	tmpParams.threshold=0;
	sgConfig.AddDescParams(tmpParams);
	sgConfig.SetMinSegmentLength(1);
	Segmentator mySegmentator(sgConfig);
	mySegmentator.Start();

	//Segmentate
	mySegmentator.Do(mySegment);
	  
	if(displaySegments)
		DisplaySegments(mySegment,samplingRate);


	//Now I will try to segmentate the first note into Attack/Steady_State/Release
	
	mySegmentator.Stop();

	sgConfig.DefaultInit();
	tmpParams.id=SpectralEnergyId;
	tmpParams.percentil=10;
	tmpParams.threshold=0.0016;
	sgConfig.AddDescParams(tmpParams);
	sgConfig.SetMinSegmentLength(0);
	mySegmentator.Configure(sgConfig);
	mySegmentator.Start();
	
	for(i=0;i<mySegment.GetChildren().Size();i++)
	{
		mySegmentator.Do(mySegment.GetChildren()[i]);

		if(displaySegments)
		{
			SegmentConfig subSegmentCfg=mySegment.GetChildren()[i].GetConfig();
			subSegmentCfg.SetHoldsData(true);
			mySegment.GetChildren()[i].Configure(subSegmentCfg);
			DisplaySegments(mySegment.GetChildren()[i],samplingRate);
		}
	}
	std::cout << "Segmentation finished, starting XML dump \n";
	  
#ifdef CLAM_USE_XML
		
	// Removing segment data (we don't need to dump it into XML)
	segmentCfg.SetHoldsData(false);
	mySegment.Configure(segmentCfg);

		

	/*****Storing result to XML document*****/ 
	XMLStorage storage ("XML-Document");
	XMLComponentAdapter myAdapter(mySegment,"AudioSegment",true);
	 
	storage.Store(&myAdapter);
	  
	std::fstream fileout("test.xml", std::ios::out);
	  
	storage.dumpOn(fileout);
#endif//CLAM_USE_XML

}

void DisplaySegments(Segment& root, TData samplingRate)
{
	//GUI objects
	Builder*		GUIBuilder = CLAM::Builder::GetInstance("FLTK");
	GUIObject* myGUI = GUIBuilder->GetGUI();
	int i;
	Array<TIndex> finalSegments;
	int nChildren=root.GetChildren().Size();
	if(nChildren>0)
	{
		for(i=0;i<root.GetChildren().Size();i++)
			finalSegments.AddElem((int)(root.GetChildren()[i].GetBeginTime()*samplingRate));
		finalSegments.AddElem((int)(root.GetChildren()[root.GetChildren().Size()-1].GetEndTime()*samplingRate));
		myGUI->ShowSnapshot( root.GetAudio(), finalSegments, "Segments in my Audio", 255, 255, 255 );
	}
	else myGUI->ShowSnapshot( root.GetAudio());
}
