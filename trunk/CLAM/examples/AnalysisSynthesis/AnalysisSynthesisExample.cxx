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

#include "AnalysisSynthesisExampleBase.hxx"

#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"
#include <iostream>

using namespace CLAM;
using namespace std;

class StdOutProgress:public Progress
{
public:
	int mLastVal;
		
	StdOutProgress(const char* title,float from,float to)
	:Progress(title,from,to)
	{
		mLastVal=0;
		std::cout << mTitle << "\n";
	}
	~StdOutProgress()
	{
		std::cout << "Done\n";
	}
	void Update(float val)
	{
		int ival = int(100.*(val-mFrom)/(mTo-mFrom));
		if (ival==mLastVal) return;
		mLastVal = ival;
		std::cout << mLastVal << "% \n";
	}
};

class StdOutWaitMessage:public WaitMessage
{
public:
	StdOutWaitMessage(const char* title)
	:WaitMessage(title)
	{
		std::cout << mTitle << "\n";
	}
	~StdOutWaitMessage()
	{
		std::cout << "Done\n";
	}
};

class AnalysisSynthesisExampleStdio:public AnalysisSynthesisExampleBase
{
public:
	CLAM::Progress* CreateProgress(const char* title,float from,float to)
	{
		return new StdOutProgress(title,from,to);
	}
	CLAM::WaitMessage* CreateWaitMessage(const char* title)
	{
		return new StdOutWaitMessage(title);
	}

	void Run(void);
};

void AnalysisSynthesisExampleStdio::Run(void)
{
	bool finish = false;
	while(!finish){
		std::cout <<"\n" << "\n";
		std::cout << "SMS Analysis/Synthesis Aplication" << "\n";
		std::cout << "MTG - UPF (Barcelona, Spain)"<<"\n" << "\n";
		std::cout << "Please choose one of the following options:" << "\n" << "\n";
		std::cout << "1. Load Analysis/Synthesis configuration file" << "\n";
		std::cout << "2. Load previously analyzed SMS file" << "\n";
		std::cout << "3. Analyze" << "\n";
		std::cout << "4. Store Analysis File" << "\n";
		std::cout << "5. Analyze Melody" << "\n";
		std::cout << "6. Store analyzed Melody" << "\n";
		std::cout << "7. Load Transformation Score" << "\n";
		std::cout << "8. Transform" << "\n";
		std::cout << "9. Synthesize" << "\n";
		std::cout << "10. Finish" << "\n" << "\n";

		int option;
		std::cin>>option;

		switch(option)
		{
			case 1://Load configuration
			{
				std::string inputXMLFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputXMLFileName;
				LoadConfig(inputXMLFileName);
				break;
			}
			case 2://Load analysis data
			{
				std::string inputXMLFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputXMLFileName;
				LoadAnalysis(inputXMLFileName);
				break;
			}
			case 3://Analyze
			{
				if(!mHaveConfig||!mHaveAudioIn)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				LoadInputSound();
				showSnapshotAudio(mAudioIn,"Input Audio");
				Analyze();
				break;
			}
			case 4://Store analysis data
			{
				if(!mHaveConfig)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				if(!mHaveAnalysis)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				StoreAnalysis();
				break;
			}
			case 5: //Analyze melody
			{
				if(!mHaveConfig)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				if(!mHaveAnalysis)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if(!mHaveSpectrum)
				{
					std::cout<<"\n"<<"\n"<<"Error, cannot analyze melody from loaded data"<<"\n";
					std::cout<<"\n"<<"Please select option 2 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				mHaveMelody=true;
				AnalyzeMelody();
				break;
			}
			case 6://Store melody
			{
				if(!mHaveMelody)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available melody"<<"\n";
					std::cout<<"\n"<<"Please select option 5 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				StoreMelody();
				break;
			}
			case 7://Load Transformation score
			{
				std::string inputXMLFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputXMLFileName;
				LoadTransformationScore(inputXMLFileName);
				mHaveTransformationScore = true;
				break;
			}
			case 8://Transform
			{
				if(!mHaveTransformationScore)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available transformation score"<<"\n";
					std::cout<<"\n"<<"Please select option 7 of the menu first"<<"\n";
					break;
				}
				if(!mHaveAnalysis)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				Transform();
				break;
			}

			case 9://Synthesize
			{
				if(!mHaveAnalysis)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if(!mHaveConfig)
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				Synthesize();
				
				showSnapshotAudio(mAudioOut,"Output Sound");
				showSnapshotAudio(mAudioOutSin,"Sinusoidal Component");
				showSnapshotAudio(mAudioOutRes,"Residual Component");
				break;
			}
			case 10://Exit
			{
				finish=true;
				break;
			}
			default:
			{
				std::cout<<"\n"<<"\n"<<"Error, not a valid option"<<"\n"<<"\n"<<"\n";
				break;
			}
		}	
	}
}

int main(int argc,char** argv)
{
	try{
		AnalysisSynthesisExampleStdio example;
		example.Run();
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

/*
void GenerateXML()
{
	SMSAnalysisSynthesisConfig c;
	XMLStorage x;
	x.Dump(c,"SMSAnalysisSynthesisConfig","c:\\config.xml");
}

TData Error(Audio& original, Audio& synthesized,Audio& error)
{
	CLAM_ASSERT(original.GetSize()==synthesized.GetSize(),"Original and synthesized audio do not have the same size");
	TData err=0;
	int size=original.GetSize();
	for(int i=0;i<size;i++)
	{
		err+=error.GetBuffer()[i]=(original.GetBuffer()[i]-synthesized.GetBuffer()[i]);
	}
	return err;
}
*/
