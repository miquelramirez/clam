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
#include "TraverseDirectory.hxx"

#include <iostream>
#include <algorithm>


namespace CLAM{

/** Declaration of the concrete Traverse directory class to apply to all files*/
class SMSBatch:public TraverseDirectory,public SMSBase
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
	void OnFile(const std::string& filename);
	void Run(void);

private:
	std::string ChangeExtension(const std::string& filename,const std::string& newExtension);
	int mOption;
};


//Auxiliary function to change the .mid extension to .xml in a given filename
std::string SMSBatch::ChangeExtension(const std::string& filename,const std::string& newExtension)
{
	std::string result=filename.substr(0,filename.rfind('.')+1);
	result.append(newExtension);
	return result;
}


//Function where the process related to every file is actually implemented
void SMSBatch::OnFile(const std::string& filename)
{      
	std::string xml("xml");
	
	//First we ensure that it is an XML file looking at the extension
	if(GetExtension(filename)!=xml) 
		return;
	std::cout<<"\n"<<"Processing file: "<<filename<<"\n";
	LoadConfig(filename);
	switch(mOption)
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

		mOption=0;
		std::cin>>mOption;

		if (!mOption) return;
		
		std::string folderName;
		std::cout<<"\n"<<"\n"<<"Enter folder name where xml configuration files are located (note that all subdirectories will be scanned recursively): \n";
		std::cin>>folderName;
		
		Traverse(folderName);

	}
}

};//namespace CLAM

int main(int argc,char** argv)
{
	try{
		CLAM::SMSBatch example;
		example.Run();
	}
	catch(CLAM::Err error)
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

