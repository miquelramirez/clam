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


#include "directory.h"
#include <iostream>
#include <algorithm>

using namespace CLAM;
using namespace std;

class SMSBatch:public SMSBase
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

void SMSBatch::Run(void)
{
	bool finish = false;
	while(!finish){
		std::cout <<"\n" << "\n";
		std::cout << "SMS Analysis/Synthesis Batch Aplication" << "\n";
		std::cout << "MTG - UPF (Barcelona, Spain)"<<"\n" << "\n";
		std::cout << "Please choose one of the following options:" << "\n" << "\n";
		std::cout << "1. Analyze and Synthesize" << "\n";
		std::cout << "2. Only Analyze" << "\n";
		std::cout << "3. Only Synthesize" << "\n";
		std::cout << "0. Finish" << "\n" << "\n";

		int option;
		std::cin>>option;

		if(option==0)
		{
			finish=true;
			break;
		}
		
		std::string folderName;
		std::cout<<"\n"<<"\n"<<"Enter folder name where xml configuration files are located: \n";
		std::cin>>folderName;
		
		using namespace boost::filesystem;

		std::string inputConfigFileName;

		for (dir_it it(folderName); it != dir_it(); ++it){
			if (!get<is_hidden>(it))
			{
				if(!get<is_directory>(it))
				{
					inputConfigFileName=*it;
					std::string ext=inputConfigFileName.substr(inputConfigFileName.length()-4,inputConfigFileName.length());
					if(ext!=".xml") break;
					
					std::string fullPathConfigName=folderName+'/'+inputConfigFileName;
					std::cout <<"Processing configuration file: "<<fullPathConfigName<<"\n";
					LoadConfig(fullPathConfigName);
					switch(option)
					{
						case 1://Analyze, synthesize and store output sound + sinusoidal componet + residual component
						{
							LoadInputSound();
							Analyze();
							Synthesize();
							StoreOutputSound();
							StoreOutputSoundSinusoidal();
							StoreOutputSoundResidual();
							break;
						}
						case 2://Analyze content of a given folder and store output .sdif or .xml files
						{
							LoadInputSound();
							Analyze();
							StoreAnalysis( mGlobalConfig.GetOutputAnalysisFile() );			
							break;
						}
						case 3://Synthesize previously analyzed .sdif or .xml files
						{
							LoadAnalysis(mGlobalConfig.GetInputAnalysisFile());
							Synthesize();
							StoreOutputSound();
							StoreOutputSoundSinusoidal();
							StoreOutputSoundResidual();
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
		}
	}
}

int main(int argc,char** argv)
{
	try{
		SMSBatch example;
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
