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

#ifndef _DownSampling_
#define _DownSampling_

#include "Audio.hxx"
#include "ProcessingComposite.hxx"
#include "OSDefines.hxx"
#include "../TDFilterGenerator/TDFilter.hxx"
#include "../TDFilterGenerator/TDFilterGen.hxx"


namespace CLAM
{
	class DownSamplingConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (DownSamplingConfig,2,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,TData,DownSamplingRate);
	private:
		void DefaultInit();
	};

	class DownSampling: public ProcessingComposite
	{
/*		class LowaPassFilterConfig: public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (LowaPassFilterConfig,1,ProcessingConfig);
			DYN_ATTRIBUTE(0,public,std::string,Name);
		private:
			void DefaultInit();
		};

		class LowPassFilter: public ProcessingComposite
		{
		public:
			LowPassFilter();
			~LowPassFilter();
		};*/

	public:
		DownSampling();

		DownSampling( DownSamplingConfig& cfg );

		~DownSampling();

		const char *GetClassName() const { return "DownSampling"; }

		const ProcessingConfig &GetConfig() const { return mConfig; }

		/** Supervised mode execution **/
		bool Do(void) { return true; }

		/** Unsupervised mode execution **/
		bool Do( const Audio& in, Audio &out );

		const TData& GetDownSamplinRate(void) const { return mDownSamplingRate; }

	protected:

		/** Configuration data **/
		DownSamplingConfig		mConfig;

		/** Down Sampling rate  **/
		TData					mDownSamplingRate;

		TDFilterGen				mTDFilterGenerator;

		TDFilter				mTDFilter;

		bool ConcreteConfigure( const ProcessingConfig& );

		bool ConcreteStart();

		bool ConcreteStop();

		void AttachChildren(void);

		bool ConfigureChildren();

		inline void CheckTypes( const Audio &in, const Audio &out );

		void Filter( const Audio &in, Audio &out );
		
	};
} //namespace

#endif
