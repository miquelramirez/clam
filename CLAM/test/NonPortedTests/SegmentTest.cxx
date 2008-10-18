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

#include "MonoAudioFileReader.hxx"
#include <iostream>
#include "CLAM_Math.hxx"

#include "Segment.hxx"
#include "Frame.hxx"
 
#include <fstream>
#include "XMLTestHelper.hxx"

using namespace CLAM;

#define NCHILDRENPERSEGMENT 30

void Segmentate(Segment& in)
{
	int i;
	TData beginTime=in.GetBeginTime();
	TData duration=in.GetEndTime()-in.GetBeginTime();
	TData step=duration/NCHILDRENPERSEGMENT ;
	for(i=0;i<NCHILDRENPERSEGMENT ;i++)
	{
		Segment tmpSegment;
		tmpSegment.SetBeginTime(beginTime+i*step);
		tmpSegment.SetEndTime  (beginTime+(i+1)*step);
		tmpSegment.SetpParent(&in);
		in.GetChildren().AddElem(tmpSegment);
	}

}



int main(int argc, const char **argv)
{

try{
	std::string fileName;
	if (argc<=1)
	{
		std::cout<<"Enter File Name: \n";
		std::cin>>fileName;
	}
	else
		fileName = argv[1];

	//The File I/O PO
	MonoAudioFileReaderConfig infilecfg;
	infilecfg.SetSourceFile(fileName);
	MonoAudioFileReader myAudioFileIn(infilecfg);
	

	/////////////////////////////////////////////////////////////////////////////
	// Initialization of the processing data objects :
	TSize fileSize=myAudioFileIn.GetHeader().GetSamples();
	Audio readAudio; // read Audio object 
	readAudio.SetSize(fileSize);
	TData samplingRate=readAudio.GetSampleRate();
	
	
	//Read Audio File
	myAudioFileIn.Start();
	myAudioFileIn.Do(readAudio);

	
	// Segment that will actually hold data
	Segment mySegment;
	mySegment.SetHoldsData(true);
	float duration=fileSize/samplingRate;
	std::cout << "The audio file has a total of" << duration << std::endl;
	mySegment.SetEndTime(duration);
	mySegment.SetAudio(readAudio);
	 
	//Segmentate
	std::cout << "Starting first level segmentation" << std::endl;
	Segmentate(mySegment);

	//Now I will try to segmentate the every note into Attack/Steady_State/Release
	
	int z;
	int i;
	std::cout << "Starting second level segmentation" << std::endl;
	for(i=0;i<mySegment.GetChildren().Size();i++)
	{
		std::cout << "Segmenting child number " << i << std::endl;
		Segmentate(mySegment.GetChildren()[i]);
	}
	std::cout << "Starting third level segmentation" << std::endl;
	for(i=0;i<mySegment.GetChildren().Size();i++)
	{
		std::cout << "Segmenting children of child number " << i << std::endl;
		for(z=0;z<mySegment.GetChildren()[i].GetChildren().Size();z++)
		{
			std::cout << "Segmenting child number " << i <<"."<< z << std::endl;
			Segmentate(mySegment.GetChildren()[i].GetChildren()[z]);
		}
	}
	
	//Segmentate
	std::cout << "Starting again first level segmentation" << std::endl;
	Segmentate(mySegment);

	//Access data on all segment
	std::cout << "Accessing data on all segments" << std::endl;
	for(i=0;i<mySegment.GetChildren().Size();i++)
	{
		std::cout << "Child number " << i << " has a begin time of "<< mySegment.GetChildren()[i].GetBeginTime()<<std::endl;
		for(z=0;z<mySegment.GetChildren()[i].GetChildren().Size();z++)
		{
			std::cout << "Child number " << i <<"."<< z << " has a begin time of "<< mySegment.GetChildren()[i].GetChildren()[z].GetBeginTime()<<std::endl;
		}
	}

	std::cout << "Segmentation finished, starting XML dump \n";	

	// Storing result to XML document and check if loading matches
	CLAM_ASSERT(CLAMTest::XMLInputOutputMatches(mySegment, "SegmentTest.xml"), 
		"Original resulting segment and XML reloaded one don't match");

	//Detaching all children
	std::cout << "Detaching all segments" << std::endl;
	for(i=0;i<mySegment.GetChildren().Size();i++)
	{
		for(z=0;z<mySegment.GetChildren()[i].GetChildren().Size();z++)
		{
			std::cout << "Detaching Child number " << i <<"."<< z << std::endl;
			mySegment.GetChildren()[i].GetChildren()[z].SetHoldsData(false);
		}
		std::cout << "Detaching Child number " << i << std::endl;
		mySegment.GetChildren()[i].SetHoldsData(false);
	}


}
catch(Err error)
{
	error.Print();
	std::cerr << "Abnormal Program Termination" << std::endl;
}

std::cerr << "Successfully finished" << std::endl;
return 0;

}

