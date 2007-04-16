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

#ifndef _SMSBase_
#define _SMSBase_

#include <string>

#include "SerializationController.hxx"

#include <CLAM/SMSSynthesisConfig.hxx>
#include <CLAM/SMSAnalysisConfig.hxx>
#include "SMSAnalysisSynthesisConfig.hxx"
#include <CLAM/Melody.hxx>
#include <CLAM/SMSTransformationChainConfig.hxx>
#include <CLAM/SMSTransformationChain.hxx>
#include <CLAM/SegmentDescriptors.hxx>
#include <CLAM/AudioOut.hxx>
#include "Progress.hxx"
#include "WaitMessage.hxx"
#include <CLAM/SDIFIn.hxx>
#include <CLAM/SDIFOut.hxx>

#include "SMSAppState.hxx"

namespace CLAMGUI
{
	class NullProgress : public Progress
	{
	public:
		void Update(float ) {}
		NullProgress() : Progress("",0.0,0.0) {}
	};
	
	class NullWaitMessage : public WaitMessage
	{
	public:
		NullWaitMessage() : WaitMessage("") {}
			
	};
}

namespace CLAM
{
	
	class SMSAnalysis;
	class SMSSynthesis;

	/** This is the base class for the Analysis Synthesis example. It implements
	* all the necessary processing but it cannot be instantiated. To instantiate
	* a particular analysis synthesis application you need to work directly with 
	* the GUI or stdio versions
	* @see SMSTools.hxx
	* @see SMS.hxx 
	* This class performs an SMS analysis on the input sound. Necessary inputs are
	* a configuration xml file and, in case you want to perform a transformation,
	* a transformation xml score */	
	class SMSBase
	{
	public:
		SMSBase(void);	
		virtual ~SMSBase(void);
		virtual void Run()=0;
		
		SMSTransformationChainConfig& GetCurrentTransformationScore() {
			return mTransformationScore;
		}
		
		Segment& GetOriginalSegment()
		{
			return mOriginalSegment;
		}

		Segment& GetTransformedSegment()
		{
			return mTransformedSegment;
		}
		

		void  SetCurrentTransformationScore( const SMSTransformationChainConfig& cfg )
		{
			mTransformationScore = cfg;
			GetState().SetHasTransformationScore(true);
		}

//File setter/getters

		void  SetAnalysisInputFile( const char* filename )
		{
			if ( !mGlobalConfig.HasInputAnalysisFile() )
			{
				mGlobalConfig.AddInputAnalysisFile();
				mGlobalConfig.UpdateData();
			}

			mGlobalConfig.SetInputAnalysisFile( filename );
		}

		std::string GetAnalysisInputFile() const
		{
			if ( !mGlobalConfig.HasInputAnalysisFile() )
			{
				return std::string( "" );
			}
			else				
				return mGlobalConfig.GetInputAnalysisFile();
		}

		void  SetAnalysisOutputFile( const char* filename )
		{
			if ( !mGlobalConfig.HasOutputAnalysisFile() )
			{
				mGlobalConfig.AddOutputAnalysisFile();
				mGlobalConfig.UpdateData();
			}
			mGlobalConfig.SetOutputAnalysisFile( filename );
		}

		std::string GetAnalysisOutputFile() const
		{
			if ( !mGlobalConfig.HasOutputAnalysisFile() )
				return std::string( "" );
			else
				return mGlobalConfig.GetOutputAnalysisFile();
		}

		SerializationController& GetSerializer()
		{
			return mSerialization;
		}

		Audio& GetSynthesizedSound()
		{
			return mAudioOut;
		}

		Audio& GetSynthesizedSinusoidal()
		{
			return mAudioOutSin;
		}

		Audio& GetSynthesizedResidual()
		{
			return mAudioOutRes;
		}

		Melody& GetMelody()
		{
			return mMelody;
		}
		
		SMSAnalysisSynthesisConfig& GetGlobalConfig()
		{
			return mGlobalConfig;
		}
		
		SMSAppState& GetState() {return mState;}
		

	public:
	
		/** Cleans up segment from pre-existing data*/ 
		void Flush(Segment& seg);
		/** Copies all content in src segment except Audio */
		void CopySegmentExceptAudio(Segment& src, Segment& dest);
		/** Initialize analysis and synthesis configuration from loaded
		* global configuration */
		void InitConfigs(void);
		/** Returns true if mGlobalConfig has the required elements.*/
		bool HaveCompatibleConfig();
		/** Load global configuration */
		void LoadConfig(const std::string& inputFileName);
		/** Store global configuration */
		void StoreConfig(const std::string& inputFileName);
		/** Load transformation score */
		virtual void LoadTransformationScore(const std::string& inputFileName);

		/** Store transformation score */
		void StoreTransformationScore( const std::string& outputFileName );

		bool LoadAnalysis(const char* filename);
		bool LoadAnalysis(const std::string& filename){return LoadAnalysis(filename.c_str());}

		/** Store data resulting from analysis. Some unnecessary data
		* is removed from memory */
		void StoreAnalysis(const char* filename);
		void StoreAnalysis(const std::string& filename) {StoreAnalysis(filename.c_str());}

		/** Store synthesized sound */
		virtual void StoreOutputSound(void);
		/** Store synthesized sound, only residual component */
		virtual void StoreOutputSoundResidual(void);
		/** Store synthesized sound, only sinusoidal component */
		virtual void StoreOutputSoundSinusoidal(void);
		
		/** Perform analysis. Requires a valid configuration file to be loaded */
		void Analyze(void);
		/** Perform synthesis. Requires a valid configuration file to be loaded and 
		 *	a previuos analysis or a loaded analysis */
		void Synthesize(void);
		/** Perform transformation according to previously set transformation 
		* (PitchScale by default). Requires a valid transformation score to be loaded */
		virtual void DoTransformation();
		void Transform(void);
		
		/** Load input sound */
		virtual bool LoadInputSound(void);
		/** Load sound to morph*/
		bool LoadMorphSound(void);
		
	protected:		
		/** General method for storing a sound*/
		void StoreSound(const std::string& filename,const Audio& audio);
		
		
		/** General method for loading a sound */
		virtual bool LoadSound(const std::string& filename,Segment& segment);

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoAnalysis();
		virtual void DoMorphAnalysis();

		void AnalysisProcessing();
		void MorphAnalysisProcessing();
		void SetSMSMorphFileName();

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoTracksCleanup();
		virtual void DoMorphTracksCleanup();

		void TracksCleanupProcessing();
		void MorphTracksCleanupProcessing();

		/** Perform synthesis. Requires a valid configuration file to be loaded 
		* and the analysis to be performed. */
		void SynthesisProcessing();

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoSynthesis();

		void TransformProcessing();

		void ConfigureSMSMorph();

		/** Analyze and extract melody. This feature only works on some sort of instruments
		* for monophonic phrases */
		virtual void AnalyzeMelody(void);

		void ComputeLowLevelDescriptors();

		/** Stores previously analyzed melody into xml format */
		void StoreMelody(void);


	protected:

		SMSAppState mState;
		SerializationController mSerialization;

		/** Output audio */
		Audio mAudioOut;
		/** Output audio, only sinusoidal component */
		Audio mAudioOutSin;
		/** Output audio, only residual component */
		Audio mAudioOutRes;
		
		/** Internal structure used for storing the result of the analysis, contains all data.
		* @see Segment.hxx */
		Segment mOriginalSegment;
		/** Segment that will be used for the synthesis */
		Segment mTransformedSegment;
		/** Optional segment that contains the result of the analysis of the sound to morph */
		Segment mMorphSegment;

		SegmentDescriptors mOriginalSegmentDescriptors;
		SegmentDescriptors mTransformedSegmentDescriptors;
		
		/** Analyzed melody */
		Melody mMelody;

		/** Analysis configuration */
		SMSAnalysisConfig mAnalConfig;
		/** Synthesis configuration */
		SMSSynthesisConfig mSynthConfig;
		/** Global configuration, loaded from an xml file and used to initialize
		* analysis and synthesis configurations 
		* @see InitConfigs */
		SMSAnalysisSynthesisConfig mGlobalConfig;
		/** Transformation score loaded from an xml file */
		SMSTransformationChainConfig mTransformationScore;


		/** Actual transformation to be used*/
		SMSTransformationChain mTransformation;


		/** Internal Processings used for analysis and synthesis */
		SMSAnalysis* mpAnalysis;
		SMSSynthesis* mpSynthesis;


protected:

		CLAMGUI::Progress* mCurrentProgressIndicator;
		CLAMGUI::WaitMessage* mCurrentWaitMessage;

		std::string mXMLInputFile;

		SDIFIn  mSDIFReader;


		/** Creates progress bar. Implemented both in GUI and stdio versions */
		virtual CLAMGUI::Progress* CreateProgress(const char* title,float from,float to) = 0;
		/** Creates a wait message. Implemented both in GUI and stdio versions */
		virtual CLAMGUI::WaitMessage* CreateWaitMessage(const char* title) = 0;

		void DestroyProgressIndicator();

		void DestroyWaitMessage();

		inline bool HasToDoTracksCleaning() const
		{
			return mGlobalConfig.GetDoCleanTracks();
		}

		/** sets and synchronizes sampling rate of internal objects and configurations*/
		void SetSamplingRate(TSize samplingRate);

		/** sampling rate for the whole application */
		TSize mSamplingRate;

		SMSAnalysis& GetAnalysis(){return *mpAnalysis;}
		SMSSynthesis& GetSynthesis(){return *mpSynthesis;}

		void UpdateDataInTimeStretch();
	};

};

#endif

