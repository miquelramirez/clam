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

#ifndef AudioDatabaseReader_hxx
#define AudioDatabaseReader_hxx

#include "Audio.hxx"
#include "ErrSoundFileIO.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "AudioDatabaseReaderConfig.hxx"
#include "ProcessingComposite.hxx"
#include "AudioOutPort.hxx"
#include "SMSAnalysisConfig.hxx"
#include "SMSAnalysisCore.hxx"
#include "OutPortPublisher.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"

namespace CLAM {


/**
 * Processing for audio file input. It can be configured 
 * using an AudioFileConfig Config class. Only file name 
 * and optionally file type must be configured, other 
 * fields will be set by the Processing itself once 
 * Start() has been called.
 */
class AudioDatabaseReader : public ProcessingComposite
{
public:
	class AudioDatabaseReaderConfig: public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE(AudioDatabaseReaderConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, Filename, Filename);
	protected:
		/** Dynamic type initialization: All attributes are instantiated. */
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetFilename( "" );
		}
	};


	AudioDatabaseReader();

	AudioDatabaseReader(const AudioDatabaseReaderConfig &c);

	virtual ~AudioDatabaseReader();

	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do(void);
	bool Do(Audio& in);
	bool Do(Audio& inL,Audio& inR);

protected:
	bool ConcreteStart();
	bool ConcreteStop();

private:
	AudioDatabaseReaderConfig mConfig;
	MonoAudioFileReaderConfig* myMonoAudioFileReaderConfig;
	MonoAudioFileReader* myMonoAudioFileReader;
	SMSAnalysisConfig* mySMSAnalysisConfig;
	SMSAnalysisCore* mySMSAnalysisCore;
	AudioOutPort mOutput;

	const char *GetClassName() const {return "AudioDatabaseReader";}
	void ConnectAndPublishPorts();

};

}//namespace CLAM

#endif // AudioDatabaseReader_hxx

