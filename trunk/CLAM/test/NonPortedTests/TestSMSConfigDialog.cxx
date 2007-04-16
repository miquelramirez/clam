#include <iostream>
#include "SMSContextDefDialog.hxx"
#include "SMS_SoundSourceSelection.hxx"
#include "SMS_BasicAnalysisSettings.hxx"
#include "SMS_SPpPAnalysisSettings.hxx"
#include "SMS_PeakContinuationSettings.hxx"
#include "SMSUIToolkit.hxx"
#include <FL/Fl.H>

int main( int argc, char** argv )
{
	std::cout << "Have nice shell!" << std::endl;

	SMS::ToolsUI::Toolkit::AdjustLookAndFeel();

	int desktopWidth, desktopHeight;

	SMS::ToolsUI::Toolkit::QueryDesktopMetrics( desktopWidth, desktopHeight );

	SMS::ToolsUI::SMSContextDefDialog* pMainWidget =
		new SMS::ToolsUI::SMSContextDefDialog( desktopWidth/4, desktopHeight/4, 
						       desktopWidth/2, desktopHeight/2);

	SMS::ToolsUI::SoundSourceSelection* pParams1 =
		new SMS::ToolsUI::SoundSourceSelection( );

	SMS::ToolsUI::BasicAnalysisSettings* pParams2 =
		new SMS::ToolsUI::BasicAnalysisSettings( );

	SMS::ToolsUI::SPpPAnalysisSettings* pParams3 =
		new SMS::ToolsUI::SPpPAnalysisSettings( );

	SMS::ToolsUI::PeakContinuationSettings* pParams4 =
		new SMS::ToolsUI::PeakContinuationSettings();

	pMainWidget->Tabulate( pParams1 );
	pMainWidget->Tabulate( pParams2 );
	pMainWidget->Tabulate( pParams3 );
	pMainWidget->Tabulate( pParams4 );

	pMainWidget->show();

	Fl::run();

	return 0;
}

