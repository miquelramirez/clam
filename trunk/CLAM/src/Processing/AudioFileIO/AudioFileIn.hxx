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

#ifndef _AudioFileIn_
#define _AudioFileIn_

#include "Audio.hxx"
#include "ErrSoundFileIO.hxx"
#include "AudioFileConfig.hxx"
#include "Processing.hxx"
#include "AudioOutPort.hxx"

namespace CLAM {

class SoundFileIO;

	/**
	 * Processing for audio file input. It can be configured 
	 * using an AudioFileConfig Config class. Only file name 
	 * and optionally file type must be configured, other 
	 * fields will be set by the Processing itself once 
	 * Start() has been called.
	 */
	class AudioFileIn: public Processing
	{
	protected:
		/** Audiofile Configuration */
		AudioFileConfig mConfig;
		SoundFileIO* mpSoundFileIO;
		bool mKeepFrameSizes;

		bool CheckSoundFile() throw( ErrSoundFileIO );
		bool ConcreteStart();
		bool ConcreteStop();


	private:
		AudioOutPort mOutput;

		const char *GetClassName() const {return "AudioFileIn";}

		void AddSilence(Audio&, int length);


	public:

		AudioFileIn();

		AudioFileIn(const AudioFileConfig &c);

		virtual ~AudioFileIn();

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

		/** Check if we have arrived at the EOF. */
		/** @todo there should be a more generic interface to specify that a P.O. 
		 * has reached the end of its purpose
		 */
		bool Done(void) const;

		/**
		 * Particular method to Access the Size of the file,TODO: should be implemented in a
		 * more generic way for all Processing Objects to handle information requests
		*/
		int Size() const;

		/**
		 * Particular method to Access the Sample rate of the file.
		*/
		TData SampleRate();

		/**
		 * Particular method to Access the number of channels of the file.
		*/
		int Channels();

	};
}//namespace CLAM

#endif // _AudioFileIn_

