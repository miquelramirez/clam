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

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <string>
#include "FFT_rfftw.hxx"
#include "DataTypes.hxx"
#include "ErrProcessingObj.hxx"
#include "SpectrumGView.hxx"
#include <FL/Fl.H>
#include "AudioPresentation.hxx"
#include "AudioGView.hxx"
#include "TestPresentations.hxx"

using CLAMGUITest::TestLinearMagPresentation;
using CLAMGUITest::TestLogMagPresentation;
using CLAMGUITest::TestLogPowPresentation;
using CLAMGUITest::TestPhasePresentation;

using namespace CLAM;

int main(int argc, char* argv[])
{
	try {
		
		int i,Size=1024;
		float SampleRate=8000.0f;
		
		// Audio creation
		CLAM::Audio myaudio;
		myaudio.SetSize( Size );
		myaudio.SetSampleRate( SampleRate );
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]=0.5*cos(2.0*PI*400.0*(((float)i)/SampleRate));	
		//myaudio.GetBuffer()[i] = ( (i > Size/8 ) ? 0.0f : 0.5f );

		// Spectrum attribute selection and config
		CLAM::SpecTypeFlags sflags;
		sflags.bMagPhase=true;
		sflags.bComplex=true;
		
		CLAM::SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSpectralRange( SampleRate );
		sconfig.SetSize(Size/2+1);

		// Spectrum creation
		CLAM::Spectrum myspectrum(sconfig);
		std::cout << myspectrum.GetComplexArray()[0] << std::endl;

		CLAMGUI::AtomView<Spectrum> specViewLin;
		CLAMGUI::AtomView<Spectrum> specViewLog;
		CLAMGUI::AtomView<Spectrum> specViewLogPow;
		CLAMGUI::AtomView<Spectrum> specViewPhase;
		CLAMGUI::AtomView<Audio>    auView;

		TestPhasePresentation       phasePres;
		TestLinearMagPresentation   linPres;
		TestLogMagPresentation      logPres;
		TestLogPowPresentation      logpowPres;

		AudioPresentation           audioPres;

		auView.BindTo( &myaudio );
		specViewLin.BindTo( &myspectrum );
		specViewLog.BindTo( &myspectrum );
		specViewLogPow.BindTo( &myspectrum );
		specViewPhase.BindTo( &myspectrum );

		audioPres.LinkWithView( &auView);
		phasePres.LinkWithView( &specViewPhase );
		linPres.LinkWithView( &specViewLin );
		logPres.LinkWithView( &specViewLog );
		logpowPres.LinkWithView( &specViewLogPow );

		// Processing object configuration
		CLAM::FFTConfig fconfig;
		fconfig.SetName(std::string("local.My_beautiful_fft"));
		fconfig.SetAudioSize(Size);

		// Processing object creation
		CLAM::FFT_rfftw myfft(fconfig);

		// Processing object execution
		std::cout << "Running object " << std::endl;
		myfft.Start();
		myfft.Do(myaudio,myspectrum);

		auView.Refresh();
		specViewLin.Refresh();
		specViewLog.Refresh();
		specViewLogPow.Refresh();
		specViewPhase.Refresh();

		audioPres.Show();
		linPres.Show();
		logPres.Show();
		logpowPres.Show();
		phasePres.Show();

		cout << "GUI Loop. Close the window to Exit" << endl;

		while ( Fl::wait() )
		{
			audioPres.Do();
			linPres.Do();
			logPres.Do();
			logpowPres.Do();
			phasePres.Do();
		}

	}
	catch(Err err) {
		err.Print();
	}

	return 0;

}
