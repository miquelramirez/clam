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

#ifndef _AudioFileIn_
#define _AudioFileIn_

#include "Audio.hxx"
#include "AudioFile.hxx"
#include "Processing.hxx"
#include "Port.hxx"

namespace CLAM {

class SoundFileIO;

	/** Class for audiofile Input */
	class AudioFileIn: public Processing
	{
	protected:
		/** Audiofile Configuration */
		AudioFileConfig mConfig;
		SoundFileIO* mpSoundFileIO;
		bool mKeepFrameSizes;

		bool ConcreteStart();

		bool ConcreteStop();

	private:

		const char *GetClassName() {return "AudioFileIn";}

		void AddSilence(Audio&, int length);

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:

		AudioFileIn();

		AudioFileIn(const AudioFileConfig &c);

		OutPortTmpl<Audio> Output;

		virtual ~AudioFileIn();

		/** Configuration access:
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;}

		void Attach(Audio& out) {Output.Attach(out);}

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
		bool Done(void);

		/**
		 * Particular method to Access the Size of the file,TODO: should be implemented in a
		 * more generic way for all Processing Objects to handle information requests
		*/
		int Size();

		/**
		 * Particular method to Access the Sample rate of the file.
		*/
		TData SampleRate();

		/**
		 * Particular method to Access the number of channels of the file.
		*/
		int Channels();

		// COMPONENT Methods
		/** @todo Not yet implemented. */
		void StoreOn(Storage &s) {};
 	
	};
}//namespace CLAM

#endif // _AudioFileIn_
