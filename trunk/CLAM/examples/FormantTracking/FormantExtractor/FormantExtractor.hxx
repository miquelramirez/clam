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

#ifndef _FormantExtractor_
#define _FormantExtractor_

#include "ProcessingComposite.hxx"
#include "LPC.hxx"
#include "MatrixTmpl.hxx"
#include "DownSampling.hxx"
#include "Candidates.hxx"
#include "WindowGenerator.hxx"
#include "AudioMultiplier.hxx"

namespace CLAM
{
	class FormantExtractorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FormantExtractorConfig,9,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,TData,Fmax);
		DYN_ATTRIBUTE(2,public,TData,Fmin);
		DYN_ATTRIBUTE(3,public,TData,HopSize);
		DYN_ATTRIBUTE(4,public,TData,NCandidates);
		DYN_ATTRIBUTE(5,public,TData,SamplingRate);
		DYN_ATTRIBUTE(6,public,TSize,NFrames);
		DYN_ATTRIBUTE(7,public,EWindowType,WindowType);
		DYN_ATTRIBUTE(8,public,TSize,WinSize);
	private:
		void DefaultInit();	
	};

	class FormantExtractor: public ProcessingComposite
	{
	public:
		FormantExtractor();

		FormantExtractor( FormantExtractorConfig &cfg );

		virtual ~FormantExtractor();

		const char *GetClassName() const { return "FormantExtractor"; }

		const ProcessingConfig &GetConfig() const { return mConfig; }

		/** Supervised mode execution **/
		bool Do(void) { return true; }

		/** Unsupervised mode execution **/
		bool Do( const Audio& in, Candidates &out );

		inline const TSize GetNFrames(void) const { return mNFrames; }

	protected:

		/** Configuration data **/
		FormantExtractorConfig		mConfig;

		LPC							mLPC;

		TDFilterGen					mTDFilterGen;

		DownSampling				mDownSampling;
		
		WindowGenerator				mWinGenerator;

		AudioMultiplier				mAProduct;

		Audio						mWindow;

		TSize						mNCandidates;

		TSize						mNFrames;

		TSize						mHopSize;

		TSize						mWinSize;

		TData						mSamplingRate;

		bool ConcreteConfigure( const ProcessingConfig& );

		bool ConcreteStart(void);

		bool ConcreteStop(void);

		void AttachChildren(void);

		bool ConfigureChildren(void);

		void ConfigureTDFilterGen( const DataArray& a );

		void ConfigureData(void);

		inline void CheckTypes( const Audio& in ) const;
	};
} //namespace

#endif // FormantExtractor.hxx
