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

#ifndef _Envelope_
#define _Envelope_

#include <typeinfo>
#include "DynamicType.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"
#include "GlobalEnums.hxx"


namespace CLAM{

	/** Configuration class for Envelope data objects.
	 *  @see Envelope, ProcessingDataConfig*/

	class EnvelopeConfig : public ProcessingDataConfig
	{
	public:

		DYNAMIC_TYPE_USING_INTERFACE (EnvelopeConfig, 3, ProcessingDataConfig);
		/** Type of Scale used*/
		DYN_ATTRIBUTE (0, public, EScale, Scale);
		/** Size of the Break Point Function */
		DYN_ATTRIBUTE (1, public, int,    BPFSize);
		/** Information related to the type of interpolation */
		DYN_ATTRIBUTE (2, public, EInterpolation, InterpolationType);

		~EnvelopeConfig(){};
	protected:
		void DefaultInit();
		
	};

	/** Envelope class.
	 *  This class tipically represents the amplitude envelope of an
	 *  audio object. The internal representation is a BreakPoint
	 *  function (BPF). For a floating point abcisa value representing
	 *  time (in milliseconds), the BPF returns the amplitude value, in an
	 *  scale from 0.0 to 1.0.
	 *  @see ProcessingData, EnvelopeExtractor, EnvelopeConfig   */
	class Envelope : public ProcessingData
	{
	public:

		typedef BPFTmpl<TTime,TData> TEnvelopeBPF;

		DYNAMIC_TYPE_USING_INTERFACE (Envelope, 2, ProcessingData);

		/** Configuration of Envelope with information about it*/

		DYN_ATTRIBUTE (0, public, EnvelopeConfig, Config);

		/** BPF, with amplitude returned as TData values */ 

		DYN_ATTRIBUTE (1, public, TEnvelopeBPF , AmplitudeBPF); 
	public:

		/** Constructor class given a concrete configuration
		 *  @param &newConfig reference to an Envelope Configuration established by the user
		*/

		Envelope(const EnvelopeConfig &newConfig);

		/** Method that allows get the configuration of the Envelope
		 *  @return The configuration of this Envelope
		 */

		inline const ProcessingDataConfig &GetProcessingDataConfig() const;

		/** Create the configuration of Envelope using an EnvelopeConfig instantiation
		 *  @param &newConfig reference to an Envelope Configuration established by the user
		*/

		void Configure(const EnvelopeConfig &newConfig);

		/** Configure the Envelope with an EnvelopeConfig created by default*/

	protected:
		void DefaultInit();

	public:
		// Config shortcut interface.

		/** Method to obtain the size of the Break Point Function
		 *  @return Value of the size
		 */

		inline int GetBPFSize() const;

		/** Resize the BPF
		 *  @param s New size desired
		 */

		inline void SetBPFSize(int s);


		/** Method to obtain the value of Scale
		 *  @return Scale
		 */

		inline int GetScale() const;

		/** Change the Scale factor
		 *  @param s New Scale factor
		 */

		inline void SetScale(int s);
	
	};

/////////////// Inline functions

	const ProcessingDataConfig &Envelope::GetProcessingDataConfig() const
	{ 
		return GetConfig();
	};

	int Envelope::GetBPFSize() const
	{
		return GetConfig().GetBPFSize();
	}

	void Envelope::SetBPFSize(int s)
	{
		GetConfig().SetBPFSize(s);
	}

	int Envelope::GetScale() const
	{
		return GetConfig().GetScale();
	}
	void Envelope::SetScale(int s)
	{
		GetConfig().SetScale(s);
	}


}; // namespace CLAM

#endif

