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

#include "AnalysisSynthesisExampleGUI.hxx"
#include <iostream>
#include <FL/Fl.H>

namespace CLAMGUI
{

void AnalysisSynthesisExampleGUI::Run(void)
{
	Fl::get_system_colors();

	UserInterface userinterface;
	userinterface.mAnalysisSynthesisExample = this;
	Fl::get_system_colors();
	Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
	Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
	userinterface.mWindow->show();
	
	Fl::run();
}
	
Progress* AnalysisSynthesisExampleGUI::CreateProgress(const char* title,float from,float to) 
{
	ProgressGUI* tmp = new ProgressGUI(title,from,to);
	return tmp;
}

WaitMessage* AnalysisSynthesisExampleGUI::CreateWaitMessage(const char* title) 
{
	WaitMessageGUI* tmp = new WaitMessageGUI(title);
	return tmp;
}

void AnalysisSynthesisExampleGUI::ExecuteMethodOnThreadKeepingScreenUpToDate( 
	CBL::Functor0 method )
{
	mThread.SetThreadCode( method );

	mThread.Start();

	while ( mThread.IsRunning() )
	{
		Fl::wait( 0.1 );
	}
}

void AnalysisSynthesisExampleGUI::DoAnalysis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, AnalysisProcessing ) );
}

void AnalysisSynthesisExampleGUI::DoTracksCleanup()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, TracksCleanupProcessing ) );
}

void AnalysisSynthesisExampleGUI::DoSynthesis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, SynthesisProcessing ) );
}

void AnalysisSynthesisExampleGUI::DoLoadXMLAnalysis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, LoadXMLAnalysis ) );

}

void AnalysisSynthesisExampleGUI::DoLoadSDIFAnalysis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, LoadSDIFAnalysis ) );

}

void AnalysisSynthesisExampleGUI::DoStoreXMLAnalysis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, StoreXMLAnalysis ) );

}

void AnalysisSynthesisExampleGUI::DoStoreSDIFAnalysis()
{
	ExecuteMethodOnThreadKeepingScreenUpToDate( 
		makeMemberFunctor0( *this, AnalysisSynthesisExampleGUI, StoreSDIFAnalysis ) );

}



}
