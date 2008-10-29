/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _Normalization_
#define _Normalization_

#include "Processing.hxx"
#include "Audio.hxx"
#include "OutControl.hxx"

namespace CLAM
{
	/**
	Normalization is a CLAM implementation of 3 normalization's algorithms.
	-# normalizes in relation to the maximum energy value of the signal
	-# normalizes in relation to the average energy of the signal
	-# scales the audio by the value under which lies 90% of the energy values that are not "silence"

	The class's Do() function input is CLAM Audio, the ouput is the normalized Audio.

	The class's configuration parameters are:
	<table>
	<tr>
		<th>parameters</th>
		<th>default value)</th>
		<th>Comment</th>
	</tr>
	<tr>
		<th>FrameSize</th>
		<td>2205 - 0.05s @ 44.1 kHz</td>
		<td>Frame Size over which normalization takes place</td>
	</tr>
	<tr>
		<th>Type</th>
		<td>(1)</td>
		<td>Type of normalization to be applied. 1, 2 or 3 according to 
		the normalizations descriptions above.</td>
	</tr>


	@author jricard@iua.upf.es
	@author gpeter@iua.upf.es

*/

	class NormalizationConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (NormalizationConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, Type);
		DYN_ATTRIBUTE (1, public, TSize, FrameSize);

		void DefaultInit(void);
	};
	
	class Normalization: public Processing
	{
	private:
		NormalizationConfig mConfig;

		/**
		 * This control will send a signal for each Do execution, telling if the passed audio is silence (0) or not (1)
		 * */
		FloatOutControl mIsSilenceCtrl;

		int mType;
		TSize mFrameSize;
		
		bool ConcreteConfigure(const ProcessingConfig& c);

		/*
		 * With this check an app can know if the passed audio is silence, 
		 * just consulting the value of the control.
		 */
		void CheckSilence( int size );

	public:

		Normalization();
		Normalization(NormalizationConfig& c);
		~Normalization();

		const char * GetClassName() const {return "Normalization";}

		bool Do(void);

		bool Do(Audio &in);
		bool Do( const Audio& unnorm, Audio& norm );
		
		const ProcessingConfig &GetConfig() const { return mConfig;}

	protected:
		
		TData ComputeScaleFactorFromMaxEnergy( DataArray& inAudio );
		TData ComputeScaleFactorFromAvgEnergy( DataArray& inAudio );
		TData ComputeScaleFactorFromDominantEnergy( DataArray& inAudio );

	};
	
}

#endif


