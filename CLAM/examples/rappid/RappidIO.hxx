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

#ifndef _RAPPID_IO_H
#define _RAPPID_IO_H

#include "Enum.hxx"
#include "AudioFile.hxx"
#include "Processing.hxx"
#include "RappidTypes.hxx"

using namespace CLAM;

class ERappidIOType : public Enum {
public:
		
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	ERappidIOType() : Enum(sEnumValues, sDefault) {}
	ERappidIOType(tValue v) : Enum(sEnumValues, v) {};
	ERappidIOType(std::string s) : Enum(sEnumValues, s) {};
		
	typedef enum {
		eRealTime,
		eFile,
		eRealTimeAndFile,
		eWaveform
	};
		
	virtual Component* Species() const
		{
			return (Component*) new ERappidIOType(eRealTime);
		};
};

class RappidIOConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (RappidIOConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string,     Name);
	DYN_ATTRIBUTE (1, public, RappidAppConfig, GlobalConfig);
	DYN_ATTRIBUTE (2, public, ERappidIOType,   IOType);
	DYN_ATTRIBUTE (3, public, std::string,     FileName);
	DYN_ATTRIBUTE (4, public, std::string,     AudioDevice);
	DYN_ATTRIBUTE (5, public, int,             AudioDeviceChannelID);

	void Dump();

protected:
	void DefaultInit(void);
};

namespace CLAM {

	class AudioManager;
	class ProcessingConfig;

}

class RappidIOBase {

	static CLAM::AudioManager *pAudioManager;

	static TData   mRTRequestedSampleRate;

	static TData   mRTSampleRate;

	ERappidIOType  mType;

	TData          mSampleRate;

	int            mFrameSize;

protected:

	RappidIOConfig mConfig;

	static bool AudioManagerStarted;

	RappidIOBase();

	CLAM::AudioManager *AudioManagerSingleton();

	void StartAudioManager();

	void BaseConfigure(const ProcessingConfig &cfg);

public:

	static EAudioFileType FileTypeFromName(const std::string &name);

	static TData RealTimeSampleRate() {return mRTSampleRate;}

	TData SampleRate() const {return mSampleRate;}

	int   FrameSize() const {return mFrameSize;}

	ERappidIOType Type() const {return mType;}

	static void DumpCommon();

};

template<class IOCLASS>
class RappidIO : public RappidIOBase {

protected:

	virtual ~RappidIO() {}

	virtual void Adopt(Processing&) = 0;

	virtual void ConfigError(const std::string &str) = 0;

	template<class PO, class CFG>
	bool CreateAndConfigure(PO** po, CFG &cfg)
	{
		cfg.SetSampleRate(int(SampleRate()));
		cfg.SetFrameSize(FrameSize());
		if (!*po) {
			*po = new PO(cfg);
			if (*po)
				Adopt(**po);
			else {
				ConfigError("Memory full");
				return false;
			}
		}
		if (!(*po)->Configure(cfg)) {
			ConfigError((*po)->GetStatus());
			return false;
		}
		return true;
	}
};

#endif
