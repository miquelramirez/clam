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

#ifndef _EnvelopeModulator_
#define _EnvelopeModulator_

#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "Envelope.hxx"
#include "Audio.hxx"

namespace CLAM
{

	class EnvModulatorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (EnvModulatorConfig, 4, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TData, SampleRate);
		DYN_ATTRIBUTE (2, public, int,  FrameSize);
		DYN_ATTRIBUTE (3, public, bool, EnvelopeCompression);
	protected:

		void DefaultInit(void);
	};
	
	class EnvelopeModulator: public Processing
	{
	public:

		InPortTmpl<Envelope> InputEnvelope;
		InPortTmpl<Audio>    InputAudio;
		OutPortTmpl<Audio>   Output;

	private:
		EnvModulatorConfig mConfig;
		TTime mDeltaX;
		bool mCompress;

		TData Compress(TData);

		bool ConcreteConfigure(const ProcessingConfig& c);

	public:
		EnvelopeModulator(const EnvModulatorConfig& c = EnvModulatorConfig());

		const char * GetClassName() const {return "EnvelopeModulator";}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		void Attach(Envelope& env, Audio& inp, Audio& out);

		bool Do(void);

		bool Do(const Envelope& env, const Audio& inp, Audio& out) throw(ErrProcessingObj);

		void StoreOn(Storage &s) {};
	};
	
}

#endif


