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

#include "SMSBase.hxx"
#include <iostream>
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"


using namespace CLAM;
using namespace std;

class SMSStdio:public SMSBase
{
public:
	CLAMGUI::Progress* CreateProgress(const char* title,float from,float to)
	{
		return new CLAMGUI::StdOutProgress(title,from,to);
	}
	CLAMGUI::WaitMessage* CreateWaitMessage(const char* title)
	{
		return new CLAMGUI::StdOutWaitMessage(title);
	}

	void Run(void);
};

void SMSStdio::Run(void)
{
	bool finish = false;
	while(!finish){
		std::cout <<"\n" << "\n";
		std::cout << "SMSBase Analysis/Synthesis Aplication" << "\n";
		std::cout << "MTG - UPF (Barcelona, Spain)"<<"\n" << "\n";
		std::cout << "Please choose one of the following options:" << "\n" << "\n";
		std::cout << "1. Load Analysis/Synthesis configuration file" << "\n";
		std::cout << "2. Load previously analyzed file" << "\n";
		std::cout << "3. Analyze" << "\n";
		std::cout << "4. Store Analysis File" << "\n";
		std::cout << "5. Analyze Melody" << "\n";
		std::cout << "6. Store analyzed Melody" << "\n";
		std::cout << "7. Load Transformation Score" << "\n";
		std::cout << "8. Transform" << "\n";
		std::cout << "9. Synthesize" << "\n";
		std::cout << "10. Store Output Sound" << "\n";
		std::cout << "0. Finish" << "\n" << "\n";

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
				if(!GetState().GetHasConfig())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
				       	break;
				}
				LoadInputSound();
				Analyze();
				break;
			}
			case 4://Store analysis data
			{
				if(!GetState().GetHasAnalysis())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				std::string storeAnalysisFile;
				std::cout<<"\n"<<"\n"<<"Enter Analysis Output File Name: [.xml|.sdif]  \n";
				std::cin>> storeAnalysisFile;
				StoreAnalysis( storeAnalysisFile );
				break;
			}
			case 5: //Analyze melody
			{
				if(!GetState().GetHasConfig())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				if(!GetState().GetHasAnalysis())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if(!GetState().GetHasSpectrum())
				{
					std::cout<<"\n"<<"\n"<<"Error, cannot analyze melody from loaded data"<<"\n";
					std::cout<<"\n"<<"Please select option 2 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				GetState().SetHasMelody(true);
				AnalyzeMelody();
				break;
			}
			case 6://Store melody
			{
				if(!GetState().GetHasMelody())
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
				GetState().SetHasTransformationScore(true);
				break;
			}
			case 8://Transform
			{
				if(!GetState().GetHasTransformationScore())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available transformation score"<<"\n";
					std::cout<<"\n"<<"Please select option 7 of the menu first"<<"\n";
					break;
				}
				if(!GetState().GetHasAnalysis())
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
				if(!GetState().GetHasAnalysis())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if(!GetState().GetHasConfig())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				Synthesize();
				
				break;
			}
			case 10://Store Synthesized Sounds
			{
				if(!GetState().GetHasAudioOut())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available synthesis"<<"\n";
					std::cout<<"\n"<<"Please select option 9 of the menu first"<<"\n";
					break;
				}
				StoreOutputSound();
				StoreOutputSoundSinusoidal();
				StoreOutputSoundResidual();
				break;
			}
			case 0://Exit
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
		SMSStdio example;
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
	SMSBaseAnalysisSynthesisConfig c;
	XMLStorage::Dump(c,"SMSBaseAnalysisSynthesisConfig","c:\\config.xml");
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

