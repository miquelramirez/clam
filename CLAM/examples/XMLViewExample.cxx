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
#include <string>
#include <fstream>
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::ofstream;

#include "FFT_rfftw.hxx"
#include "DataTypes.hxx"
#include "ErrProcessingObj.hxx"

#include "XMLStorage.hxx"

#include "CBL.hxx"

#include "ProcObjView.hxx"
#include "ProcDataView.hxx"
#include "Presentation.hxx"
#include "ViewConfiguration.hxx"
#include "GView.hxx"

using CLAMGUI::View;
using CLAM::Storage;
using CLAM::XMLStorage;
using CLAMGUI::Aspect;
using CLAMGUI::Presentation;

class XMLPresentation
	: public Presentation
{

	typedef CLAMGUI::ServiceCallback<View::GenericServices::XMLDump>::Type XMLSignal;

public:

	XMLPresentation(char* root_name)
		: Presentation(), mStorage( root_name )
		{
			Init();
		}

	~XMLPresentation()
		{
		}

	void PublishCallbacks();

	void DumpOnFile( string file_name );

protected:

	void Init();

	XMLStorage& ProvideStorage();

private:

	XMLSignal                 mXMLCb;
	XMLStorage                mStorage;
};

XMLStorage& XMLPresentation::ProvideStorage()
{
	return mStorage;
}

void XMLPresentation::Init()
{
	mXMLCb = makeMemberFunctor0R( Storage& , *this, XMLPresentation, ProvideStorage );
}

void XMLPresentation::PublishCallbacks()
{
	Aspect cfg;

	cfg.AddRequest( View::GenericServices("XMLDump"), &mXMLCb );

	mLinkedView->Configure( cfg );
}

void XMLPresentation::DumpOnFile( string file_name )
{
	mLinkedView->Refresh(); // We do the storeOn...
	ofstream output( file_name.c_str() );
	mStorage.dumpOn( output );
}


using namespace CLAM;

typedef CLAMGUI::ProcDataView<Spectrum> SpectrumGenericView;
typedef CLAMGUI::ProcDataView<Audio>    AudioGenericView;
typedef CLAMGUI::ProcObjView<FFT_rfftw> FFTGenericView;

int main(int argc, char* argv[])
{
	try {
		
		int i,Size=1024;
		float SampleRate=8000.0;
		
		// Audio creation
		CLAM::Audio myaudio;
		myaudio.SetSize( Size );
		for (i=0;i<Size;i++)
			myaudio.GetBuffer()[i]=0.5*sin(2.0*PI*400.0*(((float)i)/SampleRate));

		AudioGenericView audioView;
		XMLPresentation xmlAudioPres("XML_View_Example_Audio");

		audioView.BindTo( &myaudio );
		xmlAudioPres.LinkWithView( &audioView );

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
		
		SpectrumGenericView specView;
		XMLPresentation xmlSpecPres( "XML_View_Example_Spectrum" );
		//SpectrumPresentation pres;

		specView.BindTo( &myspectrum );
		xmlSpecPres.LinkWithView( &specView );

		// Processing object configuration
		CLAM::FFTConfig fconfig;
		fconfig.SetName(std::string("local.My_beautiful_fft"));
		fconfig.SetAudioSize(Size);
		
		// Processing object creation
		CLAM::FFT_rfftw myfft(fconfig);
		
		FFTGenericView myfft_view;
		XMLPresentation xmlFFTPres ("XML_View_Example_FFT");

		myfft_view.BindTo( &myfft );
		xmlFFTPres.LinkWithView( &myfft_view );

		// Processing object execution
		std::cout << "Running object " << std::endl;
		myfft.Start();
		myfft.Do(myaudio,myspectrum);
		
		
		cout << "Dumping XML...." << endl;

		xmlAudioPres.DumpOnFile( "Audio.xml" );
		xmlSpecPres.DumpOnFile( "Spectrum.xml" );
		xmlFFTPres.DumpOnFile( "FFT_rfftw.xml" );

		cout << "Done!" << endl;

	}
	catch(Err err) {
		err.Print();
	}
	
	return 0;
	
}
