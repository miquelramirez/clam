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

#ifndef _ENVELOPE_MORPHER_H
#define _ENVELOPE_MORPHER_H

#include "ProcessingComposite.hxx"
#include "AudioFileOut.hxx"
#include "EnvelopeModulator.hxx"
#include "EnvelopeExtractor.hxx"
#include "EnvelopeGenerator.hxx"
#include "RappidTypes.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"


using namespace CLAM;

class EnvelopeMorpherConfig: public ProcessingConfig {
public:
	DYNAMIC_TYPE_USING_INTERFACE(EnvelopeMorpherConfig,8,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string,     Name);
	DYN_ATTRIBUTE (1, public, RappidAppConfig, GlobalConfig);
	DYN_ATTRIBUTE (2, public, EInterpolation,  InterpolationType);
	DYN_ATTRIBUTE (3, public, ControlConfig,   InterpolationPeriod);
	DYN_ATTRIBUTE (4, public, ControlConfig,   IntegrationLength);
	DYN_ATTRIBUTE (5, public, ControlConfig,   NormalLevel);
	DYN_ATTRIBUTE (6, public, ControlConfig,   SilenceLevel);
	DYN_ATTRIBUTE (7, public, std::string,     EnvelopeFile);
	void DefaultInit(void);
	void Dump(void);
};


class EnvelopeMorpher: public ProcessingComposite {

public:

	InPortTmpl<Audio>  InputAudio;
	InPortTmpl<Audio>  ControlAudio;
	OutPortTmpl<Audio> OutputAudio;

	InControlTmpl<EnvelopeMorpher> cInterpolationPeriod;
	InControlTmpl<EnvelopeMorpher> cIntegrationLength;
	InControlTmpl<EnvelopeMorpher> cNormalLevel;
	InControlTmpl<EnvelopeMorpher> cSilenceLevel;

private:

	EnvelopeMorpherConfig   mConfig;

	CLAM::EnvelopeExtractor mAnalysis;
	CLAM::EnvelopeModulator mModulator;

	CLAM::Audio             mEnvelopeAudio;
	CLAM::Envelope          mAudioEnvelope;

	CLAM::EnvelopeGenerator *pEnvelopeGen;
	CLAM::AudioFileOut      *pEnvelopeFile;

	TData mSampleRate;
	bool mDumpEnvelope;

	void AdoptChildren();

	void AttachChildren();

	bool ConfigureChildren();

	void ConfigureAudio(Audio&);

	bool ConfigureData();

	void InitializeData();

	bool ConcreteConfigure(const ProcessingConfig&);

	bool ConcreteStart();

	void InitializeControls();

	int InterpolationPeriodChange(TControlData val);

	int IntegrationLengthChange(TControlData val);

	int NormalLevelChange(TControlData val);

	int SilenceLevelChange(TControlData val);


public:

	EnvelopeMorpher();

	EnvelopeMorpher(const EnvelopeMorpherConfig &);

	~EnvelopeMorpher();

	bool Do(void);

	void Attach(Audio& in, Audio& ctl, Audio &out);

	bool Do(Audio& in, Audio& ctl, Audio &out);

	const ProcessingConfig &GetConfig() const {return mConfig;}

	TData NormalLevel() const {return mAnalysis.NormalLevelControl();}

	TData SilenceLevel() const {return mAnalysis.SilenceLevelControl();}

	TTime InterpolationPeriod() const {return mAnalysis.InterpolationPeriodControl();}

	TTime IntegrationLength() const {return mAnalysis.IntegrationLengthControl();}

};

#endif
