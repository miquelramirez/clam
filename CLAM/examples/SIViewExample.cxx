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

#include "WidgetTKWrapper.hxx"
#include "SpectrumGView.hxx"
#include "AudioGView.hxx"
#include "AudioPresentation.hxx"
#include "SpectrumPresentation.hxx"
#include "GeometryKit.hxx"

using CLAMGUI::WidgetTKWrapper;
using CLAMGUI::Geometry;

using namespace CLAM;

typedef CLAMGUI::ProcDataView<Spectrum> SpectrumView;
typedef CLAMGUI::ProcDataView<Audio>    AudioView;
typedef CLAMGUI::ProcDataPresentation<Spectrum> SpectrumPresentation;
typedef CLAMGUI::ProcDataPresentation<Audio> AudioPresentation;

int main(int argc, char* argv[])
{
	try {
		
		int i,Size=1024;
		float SampleRate=8000.0;
		
		WidgetTKWrapper* toolkit = WidgetTKWrapper::GetWrapperFor("FLTK");

		// Audio creation
		CLAM::Audio myaudio;
		myaudio.SetSize( Size );
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]=0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate));

		AudioView audioView;
		AudioPresentation audio_pres(	Geometry( 10, 100, 512, 384 ),
										"An Audio Processing Data Presentation" );

		audioView.BindTo( &myaudio );
		audio_pres.LinkWithView( &audioView );

		// Spectrum attribute selection and config
		CLAM::SpecTypeFlags sflags;
		sflags.bMagPhase=true;
		sflags.bComplex=true;
		
		CLAM::SpectrumConfig sconfig;
		sconfig.SetType(sflags);
		sconfig.SetSize(Size/2+1);
		
		// Spectrum creation
		CLAM::Spectrum myspectrum(sconfig);
		std::cout << myspectrum.GetComplexArray()[0] << std::endl;
		
		SpectrumView specView;
		SpectrumPresentation spectrum_pres(	Geometry( 562, 100, 512, 384 ),
									"An Spectrum Processing Data Presentation" );
		//SpectrumPresentation pres;

		specView.BindTo( &myspectrum );
		spectrum_pres.LinkWithView( &specView );

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
		
		audioView.Refresh();
		specView.Refresh();

		spectrum_pres.Show();
		audio_pres.Show();

		cout << "GUI Loop. Close the window to Exit" << endl;

		do
		{
//			spectrum_pres.Do();
//			audio_pres.Do();
			toolkit->Tick();
		} while ( !toolkit->IsClosing() );
		
	}
	catch(Err err) {
		err.Print();
	}
	
	return 0;
	
}
