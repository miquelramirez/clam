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

#ifndef _AudioFileOut_
#define _AudioFileOut_

#include "Audio.hxx"
#include "AudioFileConfig.hxx"
#include "Processing.hxx"
#include "AudioInPort.hxx"



namespace CLAM {

	class SoundFileIO;

	/** Class for audiofile output */
	class AudioFileOut: public Processing
	{
	protected:
		/** Audiofile Configuration */
		AudioFileConfig mConfig;
		SoundFileIO* mpSoundFileIO;
	private:


		AudioInPort	mInput;

		const char *GetClassName() const {return "AudioFileOut";}

		/** Configuration change method
		 * @pre argument should be an AudioFileOutConfig
		 */

		bool ConcreteConfigure(const ProcessingConfig&);

		bool ConcreteStart();

		bool ConcreteStop();

	public:

		AudioFileOut();

		AudioFileOut(const AudioFileConfig &c);

		virtual ~AudioFileOut();

		/** Configuration access:
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;}

		/** Supervised-mode Do function.
		 */
		bool Do(void);

		/** Standard FFT Do function, with storage class references as
		 * arguments. This method implements the old conversor routines.
		 */
		bool Do(const Audio& in);

		bool Do(const Audio& inL,const Audio& inR);

	};
};//namespace CLAM

#endif // _AudioFileOut_

