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

#include "SpectralAnalysisLite.hxx"
#include "SpectralSynthesisLite.hxx"
#include "StreamBuffer.hxx"
#include "CircularStreamImpl.hxx"
#include "TestUtils.hxx"
#include <iostream>

using CLAM::Spectrum;
using CLAM::Audio;
using CLAM::SpectralAnalysis;
using CLAM::SpectralAnalysisConfig;
using CLAM::SpectralSynthesis;
using CLAM::SpectralSynthesisConfig;

using namespace std;
using namespace CLAMTest::TestUtils;

namespace CLAMTest {

	class TestAnalysisSynthesis
	{
		int mDataLength;
		int mInputAudioSize;
		int mTransformAudioSize;
		int mSpectralSize;
		int mHopSize;

		SpectralAnalysisConfig*  acfg1;
		SpectralAnalysis*        analysis1;
		SpectralSynthesisConfig* scfg1;
		SpectralSynthesis*       synthesis1;

		SpectralAnalysisConfig*  acfg2;
		SpectralAnalysis*        analysis2;
		SpectralSynthesisConfig* scfg2;
		SpectralSynthesis*       synthesis2;

		Audio mOrigAudio;
		Audio mFinalAudio;

		StreamBuffer<TData,CircularStreamImpl<TData> > *mpOutBuffer;
		WriteStreamRegion *mpWriteRegion;
		ReadStreamRegion *mpReadRegion;

		Audio    input_audio;
		Spectrum spectrum;
 		Audio    stream_write_audio;
 		Audio    stream_read_audio;

 
		void SetSConfigurationFromAnalysis(SpectralSynthesisConfig&,SpectralAnalysisConfig&);
		void BuildSynthesisPairs();
		void BuildDataObjects();

		void SetNewAConfiguration(SpectralAnalysisConfig  &);
		void SetNewSConfiguration(SpectralSynthesisConfig &);
		void GetNewAudio(Audio&,int frame);
		void SetNewAudio(Audio&,int frame);

		void TestAConstructionAndConfig();
		void TestADestruction();
		void TestSConstructionAndConfig();
		void TestSDestruction();
		void TestAnalysisSynthesisExecution();
	public:
		TestAnalysisSynthesis();
		~TestAnalysisSynthesis();
		void Do();
	};

	TestAnalysisSynthesis::TestAnalysisSynthesis() :
		mDataLength(10000),
		mInputAudioSize(1023),
		mTransformAudioSize(1023),
		mSpectralSize(mTransformAudioSize/2+1),
		mHopSize(100),
		acfg1     (0),
		analysis1 (0),
		scfg1     (0),
		synthesis1(0),
		acfg2     (0),
		analysis2 (0),
		scfg2     (0),
		synthesis2(0)
	{
		int i;
		mOrigAudio.GetBuffer().Resize(mDataLength);
		mOrigAudio.GetBuffer().SetSize(mDataLength);
		mFinalAudio.GetBuffer().Resize(mDataLength);
		mFinalAudio.GetBuffer().SetSize(mDataLength);

		CLAM_ASSERT(mDataLength > 2*mInputAudioSize,
					"TestAnalysisSynthesis(): Cuidadín, poquitos datos!");

		// The begining and the end of the test signal are zeroed,
		// so that the overlap boundary distortion is zero.

		for (i=0; i<mInputAudioSize; i++) {
			mOrigAudio.GetBuffer()[i] = 0;
			mOrigAudio.GetBuffer()[mDataLength-i-1] = 0;
		}

		for (i=0; i<(mDataLength-2*mInputAudioSize); i++)
			mOrigAudio.GetBuffer()[i+mInputAudioSize] = sin(10.0f*2.0f*TData(M_PI)*TData(i)/TData(mDataLength));
	}

	void TestAnalysisSynthesis::TestAConstructionAndConfig()
	{
		// Paranoia
		TestADestruction();

		acfg1 =     new SpectralAnalysisConfig;
		SetNewAConfiguration(*acfg1);
		analysis1 = new SpectralAnalysis;
		analysis1->Configure(*acfg1);

		acfg2 =     new SpectralAnalysisConfig(*acfg1);
		SetNewAConfiguration(*acfg2);
		analysis2 = new SpectralAnalysis(*acfg2);
	}

	void TestAnalysisSynthesis::TestADestruction()
	{
		delete acfg1;
		delete analysis1;
		delete acfg2;
		delete analysis2;
		acfg1     = 0;
		analysis1 = 0;
		acfg2     = 0;
		analysis2 = 0;
	}

	void TestAnalysisSynthesis::TestSConstructionAndConfig()
	{
		// Paranoia...
		TestSDestruction();

		// Default constructors
		scfg1 =      new SpectralSynthesisConfig;
		SetNewSConfiguration(*scfg1);
		synthesis1 = new SpectralSynthesis;
		synthesis1->Configure(*scfg1);

		// Configuration constructors
		scfg2 =      new SpectralSynthesisConfig(*scfg1);
		SetNewSConfiguration(*scfg2);
		synthesis2 = new SpectralSynthesis(*scfg2);
	}

	void TestAnalysisSynthesis::TestSDestruction()
	{
		delete scfg1;
		delete synthesis1;
		delete scfg2;
		delete synthesis2;
		scfg1      = 0;
		synthesis1 = 0;
		scfg2      = 0;
		synthesis2 = 0;
	}

	void TestAnalysisSynthesis::TestAnalysisSynthesisExecution()
{
		int i;
		int n_frames = (mDataLength - (mInputAudioSize-mHopSize) ) / mHopSize;

		analysis1 ->Start();
		synthesis1->Start();
		for (i=0; i<n_frames; i++) {

			GetNewAudio(input_audio,i);
			analysis1 ->Do(input_audio,spectrum);

 			mpOutBuffer->GetAndActivate(mpWriteRegion,
			                            stream_write_audio.GetBuffer());
			synthesis1->Do(spectrum,stream_write_audio);
			mpOutBuffer->LeaveAndAdvance(mpWriteRegion);

			mpOutBuffer->GetAndActivate(mpReadRegion,
			                            stream_read_audio.GetBuffer());
			SetNewAudio(stream_read_audio,i);
			mpOutBuffer->LeaveAndAdvance(mpReadRegion);

		}
		analysis1 ->Stop();
		synthesis1->Stop();

		TData error = MaxDiff(mOrigAudio,mFinalAudio);

		cout << "Error: " << error << std::endl;
		
		if (error > 0.003)
			throw(TestError("Input and output audio differ."));
	}

	void TestAnalysisSynthesis::GetNewAudio(Audio& a,int frame)
	{
		int i, size = a.GetSize();
		Array<TData> &buffer = a.GetBuffer();

		Array<TData> Frame;

		mOrigAudio.GetBuffer().GiveChunk(frame*mHopSize,size,Frame);

		for (i=0; i<size; i++)
			buffer[i]=Frame[i];
	}

	void TestAnalysisSynthesis::SetNewAudio(Audio& a,int frame)
	{
		int i;
		Array<TData> &buffer = a.GetBuffer();

		Array<TData> Frame;

		mFinalAudio.GetBuffer().GiveChunk(frame*mHopSize,mHopSize,Frame);

		for (i=0; i<mHopSize; i++)
			Frame[i] = buffer[i];
	}

	void TestAnalysisSynthesis::SetNewAConfiguration(SpectralAnalysisConfig &acfg)
	{
		// TODO
		acfg.SetAnalysisWindowSize(mInputAudioSize);
		acfg.SetHopSize(mHopSize);
		acfg.SetTransformAudioSize(mTransformAudioSize);
	}

	void TestAnalysisSynthesis::SetNewSConfiguration(SpectralSynthesisConfig &scfg)
	{
		// TODO
		scfg.SetTransformAudioSize(mTransformAudioSize);
		scfg.SetAnalysisHopSize(mHopSize);
	}

	void TestAnalysisSynthesis::SetSConfigurationFromAnalysis(SpectralSynthesisConfig& scfg,
	                                                          SpectralAnalysisConfig& acfg)
	{
		// TODO
	}

	void TestAnalysisSynthesis::BuildSynthesisPairs()
	{
		if (!scfg1)
			scfg1 = new SpectralSynthesisConfig();

		SetSConfigurationFromAnalysis(*scfg1,*acfg1);

		if (!synthesis1)
			synthesis1 = new SpectralSynthesis(*scfg1);
		else
			synthesis1->Configure(*scfg1);
	}

	void TestAnalysisSynthesis::BuildDataObjects()
	{
		input_audio.SetSize(mInputAudioSize);
		spectrum.AddComplexArray();
		spectrum.RemoveMagBuffer();
		spectrum.RemovePhaseBuffer();
		spectrum.UpdateData();
		spectrum.SetSize(mSpectralSize);

		mpOutBuffer = new StreamBuffer<TData,CircularStreamImpl<TData> >
			((mSpectralSize*2-1)*3/2); // Porquesí.

		mpWriteRegion = mpOutBuffer->NewWriter(mHopSize,
		                                       mSpectralSize*2-1);
		mpReadRegion = mpOutBuffer->NewReader(mHopSize,
		                                      mHopSize);
		mpOutBuffer->Configure();
	}


	TestAnalysisSynthesis::~TestAnalysisSynthesis()
	{
		delete acfg1;
		delete analysis1;
		delete acfg2;
		delete analysis2;
		delete scfg1;
		delete synthesis1;
		delete scfg2;
		delete synthesis2;
		delete mpOutBuffer;
	}

	void TestAnalysisSynthesis::Do()
	{
		cout << "Testing analysis construction and configuration... " << flush;
		TestAConstructionAndConfig();
		cout << "ok" << endl;
		cout << "Testing analysis destruction... " << flush;
		TestADestruction();
		cout << "ok" << endl;

		cout << "Testing synthesis construction and configuration... " << flush;
		TestSConstructionAndConfig();
		cout << "ok" << endl;
		cout << "Testing synthesis destruction... " << flush;
		TestSDestruction();
		cout << "ok" << endl;

		cout << "Creating and configuring objects for execution tests... " << flush;
		TestAConstructionAndConfig();
		TestSConstructionAndConfig();
		BuildSynthesisPairs();
		BuildDataObjects();
		cout << "ok" << endl;
		
		cout << "Testing analysis/synthesis execution... " << flush;
		TestAnalysisSynthesisExecution();
		cout << "ok" << endl;

		TestADestruction();
		TestSDestruction();
	}

}

int main()
{
	try {
		CLAMTest::TestAnalysisSynthesis test;
		test.Do();
		cout << endl << "Passed." << endl;
		return 0;
	}
	catch(std::exception &e) {
		cout << e.what();
		cout << endl << "Failed." << endl;
		return 1;
	}
}

