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

#ifndef _AUDIO_COMPRESSOR_HXX
#define _AUDIO_COMPRESSOR_HXX

#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include "Audio.hxx"
#include "Enum.hxx"

namespace CLAM
{

	class ECompressionType : public Enum {
	public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		ECompressionType() : Enum(sEnumValues, sDefault) {}
		ECompressionType(tValue v) : Enum(sEnumValues, v) {};
		ECompressionType(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eExponential,
			eArctangent
		};
		
		virtual Component* Species() const
		{
			return (Component*) new ECompressionType(eExponential);
		};
	};


	class AudioCompressorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioCompressorConfig, 5, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string,      Name);
		DYN_ATTRIBUTE (1, public, TData,            SampleRate);
		DYN_ATTRIBUTE (2, public, int,              FrameSize);
		DYN_ATTRIBUTE (3, public, TTime,            Gain);
		DYN_ATTRIBUTE (4, public, ECompressionType, CompressionType);

	protected:

		void DefaultInit(void);
	};

	class AudioCompressor: public Processing
	{
	public:

		InPortTmpl<Audio>  Input;
		OutPortTmpl<Audio> Output;

	private:

		AudioCompressorConfig  mConfig;

		ECompressionType::tValue mType;

		int mFrameSize;

		TData mGain;

		template<class COMPRESSOR>
		void Compress(const Array<TData> &inp, Array<TData> &outp);

		bool ConcreteConfigure(const ProcessingConfig& c);

	public:

		AudioCompressor(const AudioCompressorConfig& c = AudioCompressorConfig());

		const char * GetClassName() const {return "AudioCompressor";}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Audio& inp, Audio& out);

};
	
}

#endif
