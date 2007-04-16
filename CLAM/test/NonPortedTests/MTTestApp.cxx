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

#include "MTTestApp.hxx"
#include "Assert.hxx"
using namespace CLAMTest;

void MTTestApp::AppCleanup()
{
	// Since observed objects are to be destroyed the views
	// should be released to avoid Segmentation Fault
	mCarView.Release();
	mModView.Release();
	mAmpModView.Release();
}

void MTTestApp::UserMain()
{
	CLAMGUI::OscillatorPresentation mCarPres( 100, 100, "Carrier");
	//CLAMGUI::OscillatorPresentation mModPres ( 100, 300, "Modulator");
 // CLAMGUI::OscillatorPresentation mAmpModPres( 100, 500, "Amplitude Modulation");
	
	mCarPres.LinkWithView( &mCarView );
 // mModPres.LinkWithView( &mModView );
//	mAmpModPres.LinkWithView( &mAmpModView );
	mCarPres.Show();
//	mModPres.Show();
//	mAmpModPres.Show();
	
	//mModView.Refresh();
	//mAmpModView.Refresh();
	
	do
	{
		
		finalize = !Fl::wait();
		
	} while ( !finalize );

}

void MTTestApp::AudioMain()
{
	// Audio buffers and output setup
	try
		{
		int SampRate = 22050.0f;

		AudioManager audioManager( SampRate );
		
		AudioIOConfig inCfgL;
		AudioIOConfig inCfgR;
		AudioIOConfig outCfgL;
		AudioIOConfig outCfgR;
		
		inCfgL.SetName("left in");
		inCfgL.SetChannelID( 0 );
		
		inCfgR.SetName("right in");
		inCfgR.SetChannelID( 1 );
		
		
		outCfgL.SetName("left out");
		outCfgL.SetChannelID(0);
		
		outCfgL.SetName("right out");
		outCfgL.SetChannelID(1);
		
		pthread_testcancel();
		
		AudioIn inL(inCfgL);
		AudioIn inR(inCfgR);
		
		AudioOut outL(outCfgL);
		AudioOut outR(outCfgR);
		
		audioManager.Start();
		
		// Oscillators configuration
	 
		CLAM::AudioDevice::TInfo info;
		inL.GetDeviceInfo( info );

		unsigned int in_samples, out_samples;

		in_samples = info.mNotifySize;
		out_samples = info.mNotifySize;

		CLAM_ASSERT( in_samples == out_samples, "We are not reading the same number of samples we are writing" );

		Audio InA;
		Audio InB;
		Audio OutA;
		Audio OutB;
		
		Audio bufA;
		Audio bufB;
		Audio bufC;
		Audio bufD;
		Audio bufF;

		InA.SetSize( in_samples );
		InB.SetSize( in_samples );
		OutA.SetSize( in_samples );
		OutB.SetSize( in_samples );

		bufA.SetSize( in_samples );
		bufB.SetSize( in_samples );
		bufC.SetSize( in_samples );
		bufD.SetSize( in_samples );			
		bufF.SetSize( in_samples );
		
		// Determining Audio sizes...
		
		// Setting up the audio buffers size
					
		OscillatorConfig carcfg;
//		OscillatorConfig modcfg;
//		OscillatorConfig ampmodcfg;
		
		AudioMultiplier mul;
		AudioAdder add;
		
		carcfg.SetSamplingRate( SampRate );
//		modcfg.SetSamplingRate( SampRate );
//		ampmodcfg.SetSamplingRate( SampRate );

		carcfg.SetFrequency(220.);
			
//		modcfg.SetFrequency(0.5);
//		ampmodcfg.SetFrequency(4);
			
//		modcfg.SetPhase(-PI/2.);
		carcfg.SetModIndex( 50.0 );
		carcfg.SetPhase( -PI/2 );
		carcfg.SetAmplitude( 0.25f );
			
//		carcfg.SetFrequency(220.);
//		modcfg.SetFrequency(220.);
//		carcfg.SetModIndex(5.);
			
		Oscillator mCar;
// 		Oscillator mMod;
// 		Oscillator mAmpMod;
			
		mCar.Configure( carcfg );
// 		mMod.Configure( modcfg );
// 		mAmpMod.Configure( ampmodcfg );
			
		// View Binding
			
		mCarView.BindTo( &mCar );
//		mModView.BindTo( &mMod );
//		mAmpModView.BindTo( &mAmpMod );

		// Starting the oscillators
		
		mCar.Start();
//		mMod.Start();
//		mAmpMod.Start();
		mul.Start();
		add.Start();
		mCarView.Refresh();

		pthread_testcancel();
		
		int k = 0;
		
		do
		{
			// Model updating... ( let's try here and see what happens... )
			mCarView.UpdateModel();
//			mModView.UpdateModel();
//			mAmpModView.UpdateModel();
			
			inL.Do( InA );
			inR.Do( InB );
//			mMod.Do(bufA);
			mCar.Do( OutB );
//			mAmpMod.Do(bufC);
		 
//			add.Do( bufA, bufB, OutB );
		 
			outL.Do( OutB );
			outR.Do( OutB );

			k++;

			pthread_testcancel();

		} while (!finalize);
			
			
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
}

