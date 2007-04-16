/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Extractor.hxx" // CLAM
#include "Pool.hxx" // CLAM
#include "DataTypes.hxx" // CLAM
#include "Spectrum.hxx" // CLAM
#include "SpectrumConfig.hxx" // CLAM
#include "Audio.hxx" // CLAM
#include "SpectralAnalysis.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM
#include <sstream>
#include <fstream>


namespace CLAMTest
{

class ProofOfConceptTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ProofOfConceptTest );

class ProofOfConceptTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ProofOfConceptTest );
	CPPUNIT_TEST(testUsingThePoolDirectly);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void assertBackToBackMatches(const CLAM::Component & result, const std::string & referenceFilename)
	{
		std::ifstream referenceFile( (GetTestDataDirectory("DescriptionPool/") + referenceFilename).c_str() );
		std::string reference(  
			(std::istreambuf_iterator<char>(referenceFile)),
			std::istreambuf_iterator<char>() );

		std::stringstream dumpedResult;
		CLAM::XmlStorage::Dump(result,"DescriptionData",dumpedResult);

		if (reference!=dumpedResult.str())
			CLAM::XmlStorage::Dump(result,"DescriptionData",referenceFilename+".failed.xml");
		CPPUNIT_ASSERT_EQUAL(reference,dumpedResult.str());
	}


	
	void testUsingThePoolDirectly()
	{
		typedef unsigned SamplePosition;
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>         ("AudioSample","Level");
		scheme.AddAttribute<SamplePosition>      ("Frame","Center");
		scheme.AddAttribute<CLAM::TData>         ("Frame","Energy");
		scheme.AddAttribute<CLAM::TData>         ("Frame","RMS");
		scheme.AddAttribute<CLAM::Spectrum>      ("Frame","SpectralDistribution");
//		scheme.AddAttribute<CLAM::Spectrum>      ("Song","EnergyCentroid");

		CLAM::DescriptionDataPool pool(scheme);


		const unsigned audioSize = 1025;
		const unsigned frameSize = 256;

		// AudioSample
		pool.SetNumberOfContexts("AudioSample",audioSize);

		// AudioSample::Level
		{
			CLAM::TData * audio = pool.GetWritePool<CLAM::TData>("AudioSample","Level");
			for (unsigned i=0; i<audioSize; i++) audio[i] = 0;
			audio[0]=1.0;
			audio[audioSize-1]=2.0;
		}

		const unsigned nFrames = audioSize/frameSize;

		// Frame
		pool.SetNumberOfContexts("Frame",nFrames);

		// Frame::Center
		{
			SamplePosition * centers = pool.GetWritePool<SamplePosition>("Frame","Center");
			for (unsigned i=0; i<nFrames; i++)
			{
				centers[i] = i*frameSize;
			}
		}

		// Frame::Energy
		{
			CLAM::TData * means = pool.GetWritePool<CLAM::TData>("Frame","Energy");
			const SamplePosition * centers = pool.GetReadPool<SamplePosition>("Frame","Center");
			const CLAM::TData * samples = pool.GetReadPool<CLAM::TData>("AudioSample","Level");
			for (unsigned i=0; i<nFrames; i++)
			{
				CLAM::TData & mean = means[i] = 0;
				const CLAM::TData * frameAudio = samples + centers[i];
				for (unsigned j=0; j<frameSize; frameAudio++, j++)
				{
					mean += (*frameAudio) * (*frameAudio);
				}
			}

		}

		// Frame::RMS
		//
		// Extractor Square
		//    TargetScope: Frame
		//    OutputBinding: .attribute(RMS)
		//    InputBinding: .attribute(Energy)
		{
			/*
			CLAM::SquareExtractor square; 
			CLAM::Extractor & extractor = square;
			extractor.BaseScope(pool,"Frame");
			extractor.OutputHook("Squared").Attribute("RMS");
			extractor.InputHook("ToBeSquared").Attribute("Energy");
			while  (extractor.Init(pool); !extractor.BeyondLastContext(); extractor.NextContext())
			{
				extractor.Compute();
			}
			*/

			CLAM::TData * RMS = pool.GetWritePool<CLAM::TData>("Frame","RMS");
			const CLAM::TData * energy = pool.GetReadPool<CLAM::TData>("Frame","Energy");
			for (unsigned i=0; i<nFrames; i++)
			{
				CLAM::TData & outData = RMS[i];
				const CLAM::TData & inData = energy[i];
				{
					outData = std::sqrt( inData );
				}
			}

		}

		// Frame::SpectralDistribution
		{
			CLAM::Spectrum * spectrums = pool.GetWritePool<CLAM::Spectrum>("Frame","SpectralDistribution");
			const SamplePosition * centers = pool.GetReadPool<SamplePosition>("Frame","Center");
			const CLAM::TData * samples = pool.GetReadPool<CLAM::TData>("AudioSample","Level");

			CLAM::SpecTypeFlags sflags;
			sflags.bMagPhase = true;
			sflags.bComplex = true;

			CLAM::SpectrumConfig     specconf;
			specconf.SetType( sflags );
			specconf.SetSpectralRange( 44100 / 2 );
			specconf.SetSize( 257 );

			CLAM::SpectralAnalysisConfig config;
			config.SetWindowSize(257);
			CLAM::SpectralAnalysis spectralAnalysis;
			spectralAnalysis.Start();
			for (unsigned i=0; i<nFrames; i++)
			{
				CLAM::Spectrum & spectrum = spectrums[i];
				CLAM::Audio audio;
				CLAM::DataArray & audioBuffer = const_cast<CLAM::DataArray & >(audio.GetBuffer());
				audioBuffer.SetPtr(const_cast<CLAM::TData *>(samples+centers[i]),frameSize);
				spectrum.Configure(specconf);
				spectralAnalysis.Do(audio, spectrum);
			}
			spectralAnalysis.Stop();
		}

		assertBackToBackMatches(pool,"SpectralAnalysis.xml");
	}


#ifdef NEVERDEFINED
	void testSubGoal()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddScope("Frame");
		scheme.AddSignalScope("AudioSample");
		scheme.AddAttribute<SignalLevel>   ("AudioSample","Level");
		scheme.AddAttribute<SampleDuration>("Frame","Center");
		scheme.AddAttribute<Spectrum>      ("Frame","SpectralDistribution");

		CLAM::Scoper * loader = CLAM::Extractor::Create("SoundLoader");
		loader.BindOutputHook("AudioSample","Level");

		CLAM::Scoper * frametizer = CLAM::Extractor::Create("Frametizer");
		frametizer.BindOutputHook("Output","Frame");
		frametizer.BindOutputHook("Center","");
		frametizer.BindInputHook("Output")

		
		CLAM::Extractor * fft = CLAM::Extractor::Create("SpectralAnalysis");
		scheme.AddExtractor(fft);

		fft.BindOutputHook("Output","Frame","SpectralDistribution");
		fft.BindInputHook("Input",
			CurrentContext().Attribute("Center").Indirect("AudioSample").Range("WindowSize")
				.Attribute("Level").RelativeRange(-framesize/2,+framesize/2)
			);

		scheme.AddExtractor(loader);
		scheme.AddExtractor(frametizer);

		scheme.SetParameter("FrameSize",256);

		CLAM::DescriptionDataPool pool(scheme);
		pool.ExtractFrom("mysong.mp3");
		CLAM::XmlStorage::Dump(pool,"Description.xml","SimacDescription");
	}
/*
----------- DescriptionScheme.xml
<Parameter name="FrameSize" type="Integer" units="SampleRange" />
...
<Attribute scope="Frame" name="Center" type="SamplePosition" />
<Attribute scope="Frame" name="SpectralDistribution" type="Spectrum" />
....
<Extractor name="SpectralAnalysis" >
	<Target scope="Frame" attribute="SpectralDistribution" />
	<Dependency 
		type="Indirect" 
		scope="Sample" 
		attribute="Signal" 
		indirectAttribute="Center"
		size = "$FrameSize"
	/>
</Extractor>
...
*/
	void testGoal()
	{
		CLAM::DescriptionScheme scheme("DescriptionScheme.xml");
		scheme.AddPlugin("DescriptionSchemeExtension.xml");
		scheme.SetParameter("FrameSize",256);

		CLAM::DescriptionDataPool pool(scheme);
		pool.ExtractFrom("mysong.mp3");
		CLAM::XmlStorage::Dump(pool,"Description.xml","SimacDescription");
	}
/*
<DescriptionDataPool NumberOfScopes='3'>
	<DescriptionScheme>
		<ImportScheme
			uri='http://www.semanticaudio.org/RecomenderDS.xml'/>
		<Attribute scope="Sample" name="Level" type="Signal" />
		<Attribute scope="Frame" name="Center" type="SamplePosition" />
		<Attribute scope="Frame" name="SpectralDistribution" type="Spectrum" />
	</DescriptionScheme>
	<ScopePool name='Sample' size='2000'>
		<AttributePool name='Level'>
			0.0 0.0 .342161 ....
		</AttributePool>"
	</ScopePool>
	<ScopePool name='Frame' size='8'>
		<AttributePool name='Center'>
			0 256 512 768 1024 1280 1536 1792
		</AttributePool>
		<AttributePool name='SpectralDistribution'>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			....
		</AttributePool>
	</ScopePool>
</DescriptionDataPool>

<DescriptionDataPool NumberOfScopes='3'>
	<DescriptionScheme>
		<ImportScheme
			uri='http://www.semanticaudio.org/RecomenderDS.xml'/>
		<Attribute scope="Sample" name="Level" type="Signal" />
		<Attribute scope="Frame" name="Center" type="SamplePosition" />
		<Attribute scope="Frame" name="SpectralDistribution" type="Spectrum" />
	</DescriptionScheme>
	<Sample size='2000'>
		<Level>
			0.0 0.0 .342161 ....
		</Level>
	</Sample>
	<Frame size='8'>
		<Center>
			0 256 512 768 1024 1280 1536 1792
		</Center>
		<SpectralDistribution>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			<Spectrum>
				0 0.7256 0.4512 0.8768 ....
			</Spectrum>
			....
		</SpectralDistribution>
	</Frame>
</DescriptionDataPool>

 */
#endif

};


} // namespace CLAMTest

