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

#ifndef _AnalysisSynthesisExampleBase_
#define _AnalysisSynthesisExampleBase_

#include <string>

#include "SMSSynthesis.hxx"
#include "SMSAnalysis.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include "Melody.hxx"
#include "SMSTransformation.hxx"
#include "SegmentDescriptors.hxx"

namespace CLAM
{

	class Progress
	{
	public:
		char* mTitle;
		float mFrom;
		float mTo;

		Progress(const char* title,float from,float to);
		virtual ~Progress();
		virtual void Update(float val) = 0;
	};

	class WaitMessage
	{
	public:
		char* mTitle;

		WaitMessage(const char* title);
		virtual ~WaitMessage();
	};


	/** This is the base class for the Analysis Synthesis example. It implements
	* all the necessary processing but it cannot be instantiated. To instantiate
	* a particular analysis synthesis application you need to work directly with 
	* the GUI or stdio versions
	* @see AnalysisSynthesisExampleGUI.hxx
	* @see AnalysisSynthesisExample.hxx 
	* This class performs an SMS analysis on the input sound. Necessary inputs are
	* a configuration xml file and, in case you want to perform a transformation,
	* a transformation xml score */	
	class AnalysisSynthesisExampleBase
	{
	public:
		AnalysisSynthesisExampleBase(void);	
		virtual ~AnalysisSynthesisExampleBase(void) { }
		void Run(void);
		/** Using this method a user-defined transformation can be set*/
		void SetTransformation(SMSTransformation* pTransformation);
	protected:
		/** Initialize analysis and synthesis configuration from loaded
		* global configuration */
		void InitConfigs(void);
		/** Load global configuration */
		void LoadConfig(const std::string& inputFileName);
		/** Load transformation score */
		void LoadTransformationScore(const std::string& inputFileName);
		/** Load previously stored analysis xml data */
		void LoadAnalysis(const std::string& inputFileName);
		/** Store data resulting from analysis. Some unnecessary data
		* is removed from memory */
		void StoreAnalysis(void);

		/** Store synthesized sound */
		void StoreOutputSound(void);
		/** Store synthesized sound, only residual component */
		void StoreOutputSoundResidual(void);
		/** Store synthesized sound, only sinusoidal component */
		void StoreOutputSoundSinusoidal(void);
		/** Load input sound */
		bool LoadInputSound(void);

		/** Perform analysis. Requires a valid configuration file to be loaded */
		void Analyze(void);
		/** Perform synthesis. Requires a valid configuration file to be loaded 
		* and the analysis to be performed. */
		void Synthesize(void);
		/** Perform transformation according to previously set transformation 
		* (PitchScale by default). Requires a valid transformation score to be loaded */
		void Transform(void);

		/** Analyze and extract melody. This feature only works on some sort of instruments
		* for monophonic phrases */
		void AnalyzeMelody(void);

		void ComputeLowLevelDescriptors();

		/** Stores previously analyzed melody into xml format */
		void StoreMelody(void);

		/** Input audio */
		Audio mAudioIn;
		/** Output audio */
		Audio mAudioOut;
		/** Output audio, only sinusoidal component */
		Audio mAudioOutSin;
		/** Output audio, only residual component */
		Audio mAudioOutRes;
		
		/** Internal structure used for analysis and synthesis, contains all.
		* @see Segment.hxx */
		Segment mSegment;
		SegmentDescriptors mSegmentDescriptors;
		
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
		SMSTransformationConfig mTransformationScore;

		/** Actual transformation to be used*/
		SMSTransformation* mpTransformation;

		/** Indicates whether there is a valid analysis-synthesis configuration */
		bool mHaveConfig;
		/** Indicates whether an analysis has been performed */
		bool mHaveAnalysis;
		/** Indicates whether there is a valid input audio */
		bool mHaveAudioIn;
		/** Indicates whether there is a valid output audio */
		bool mHaveAudioOut;
		/** Indicates whether there is a valid analyzed melody */
		bool mHaveMelody;
		/** Indicates whether there is a valid transformation score */
		bool mHaveTransformationScore;
		/** Indicates whether there is a valid spectrum, needed for melody anlysis */
		bool mHaveSpectrum;

		/** Creates progress bar. Implemented both in GUI and stdio versions */
		virtual CLAM::Progress* CreateProgress(const char* title,float from,float to) = 0;
		/** Creates a wait message. Implemented both in GUI and stdio versions */
		virtual CLAM::WaitMessage* CreateWaitMessage(const char* title) = 0;
	};

};

#endif
