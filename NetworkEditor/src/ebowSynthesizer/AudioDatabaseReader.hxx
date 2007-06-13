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

#ifndef _AudioDatabaseReader_
#define _AudioDatabaseReader_

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

class SoundFileIO;

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

		AudioDatabaseReader();

		AudioDatabaseReader(const AudioDatabaseReaderConfig &c);

		virtual ~AudioDatabaseReader();

		/** Configuration change method
		 * @pre argument should be a AudioFileInConfig
		 * NOTE: the fact that AudioFileIn::ConcreteConfigure is
		 * a public method is just a temporary measure
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		/** Configuration access:
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised-mode Do function.
		 */
		bool Do(void);

		/** Standard Do function, with storage class references as
		 * arguments. This method implements the old conversor routines.
		 */
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
//		AudioOutPort mOutput;

        const char *GetClassName() const {return "AudioDatabaseReader";}
        void ConnectAndPublishPorts();        

	};
}//namespace CLAM

#endif // _AudioDatabaseReader_

