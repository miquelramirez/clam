#include "Spectrum.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_SpectrumAnalyzer.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include "WidgetTKWrapper.hxx"
#include "WindowedSpectrumGenerator.hxx"
#include "Audio.hxx"
#include <iostream>
#include <exception>
#include <string>

using CLAMTest::WindowedSpectrumGeneratorConfig;
using CLAMTest::WindowedSpectrumGenerator;
using CLAM::Spectrum;
using CLAMVM::WidgetTKWrapper;
using CLAMVM::LogMagSpectrumAdapter;
using CLAMVM::Fl_SpectrumAnalyzer;
using CLAM::XMLStorage;

static const char* sPathToData= "./Datasets/";

bool TestKnownSineFreqUseCase( LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation )
{
		int i,Size=1024;
		float SampleRate=8000.0;
		int numIterations = 1;

		// Audio creation
		CLAM::Audio myaudio;
		myaudio.SetSize( Size );
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]= 0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate))
					+ 0.25*sin(2.0*PI*1200.0*(((float)i)/SampleRate))
					+ 0.75*sin(2.0*PI*3450.0*(((float)i)/SampleRate))
					+ 0.15*sin(2.0*PI*150.0*(((float)i)/SampleRate));

		std::cerr << "You should be seeing four sinusoids: " << std::endl;
		std::cerr << "one at 150 Hz" << std::endl;
		std::cerr << "another at 400 Hz" << std::endl;
		std::cerr << "another at 1200 Hz" << std::endl;
		std::cerr << "another at 3450 Hz" << std::endl;
		std::cerr << "If you see something different please make a bug report at http://mtg150.upf.es/mantis" << std::endl;
		std::cerr << "Thank you!" << std::endl;

		myaudio.SetSampleRate( SampleRate );

		// Spectrum attribute selection and config
		CLAM::SpecTypeFlags sflags;
		sflags.bMagPhase=true;
		sflags.bComplex=true;
		
		CLAM::SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSize(Size/2+1);
		sconfig.SetSpectralRange( SampleRate / 2 );
		
		// Spectrum creation
		CLAM::Spectrum myspectrum(sconfig);
		std::cout << myspectrum.GetComplexArray()[0] << std::endl;
		// Processing object configuration

		WindowedSpectrumGeneratorConfig wndSpecGenCfg;

		wndSpecGenCfg.SetName("SpectrumGenerator");
		wndSpecGenCfg.SetWindowType( EWindowType::eBlackmanHarris92 );
		wndSpecGenCfg.SetSampleRate( SampleRate );
		wndSpecGenCfg.SetFrameSize( Size );

		WindowedSpectrumGenerator wndSpecGen( wndSpecGenCfg );
		
		wndSpecGen.Start();
		wndSpecGen.Do( myaudio, myspectrum );
		
		view.BindTo( &myspectrum );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;
		
}

bool TestWindowedUseCase(LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "Spectrum_sine.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}


bool TestPureComplexUseCase(LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyComplex.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}

bool TestPurePolarUseCase(LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation)
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyPolar.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();

		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}

bool TestPureBPFUseCase( LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyBPF.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();

		return true;
}

bool TestBasicUseCase( LogMagSpectrumAdapter& view, Fl_SpectrumAnalyzer& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "Spectrum.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();

		return true;
}

int main( int argc, char** argv )
{
		try
		{
				LogMagSpectrumAdapter        view;
				Fl_SpectrumAnalyzer presentation( 100, 100, 640, 480, "An Spectrum Presentation");

				presentation.AttachTo( view );

				std::cerr << "BASIC SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestBasicUseCase( view, presentation ) )
						std::cerr << "Basic Use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Basic Use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY COMPLEX BUFFER ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPureComplexUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only complex buffer use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only complex buffer use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY POLAR BUFFER ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPurePolarUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only polar buffer use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only polar buffer use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY BPF BUFFERS ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPureBPFUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only BPF buffers use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only BPF buffers use case Test...... Passed!" << std::endl;
				if ( !TestWindowedUseCase( view, presentation ) )
						std::cerr << "Observing windowed spectrum  use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing windowed spectrum use case Test...... Passed!" << std::endl;

				if ( !TestKnownSineFreqUseCase( view, presentation ) )
						std::cerr << "Observing sine's spectrum with known freq use case Test .... Passed!" << std::endl;
				else
						std::cerr << "Observing sine's spectrum with known freq use case test.... FAILED!" << std::endl;


		}
		catch ( CLAM::Err& e )
		{
				std::cerr << "A CLAM controlled error has occured" << std::endl;
				e.Print();
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( std::exception& e )
		{
				std::cerr << "An standard library error has occured" << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( ... )
		{
				std::cerr << "An unknown error has occured!" << std::endl;
				return -1;
		}

		return 0;		
}

