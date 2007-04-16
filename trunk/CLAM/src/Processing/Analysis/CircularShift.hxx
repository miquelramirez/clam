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

#ifndef _CIRCULAR_SHIFT_H_
#define _CIRCULAR_SHIFT_H_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "CircularShiftConfig.hxx"
#include "InControl.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"

namespace CLAM {

	class Spectrum;
	class Audio;
	class Storage;
	class ProcessingConfig;

	/** This Processing class takes an input audio buffer and shifts it to get zero-phase
	 *	conditions in the STFT 
	 */
	class CircularShift
		: public Processing {
	protected:
		CircularShiftConfig mConfig;

		AudioInPort  mInput;
		AudioOutPort mOutput;

	protected:


		const char *GetClassName() const {return "CircularShift";}

		/** Config change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		CircularShift();

		CircularShift(const CircularShiftConfig &c);

		~CircularShift();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do( const DataArray& input, DataArray& out);
		bool Do(Spectrum& input, Spectrum& out);
		bool Do( const Audio& input, Audio& out);

		// Port interfaces.

		/** Not implemented yet */
		bool SetPrototypes(Spectrum& inputs,const Spectrum& out);
		/** Not implemented yet */
		bool SetPrototypes();
		/** Not implemented yet */
		bool UnsetPrototypes();
		/** Not implemented yet */
		bool MayDisableExecution() const {return true;}

		void SetAmount(TSize amount) { mAmount.DoControl(TControlData(amount)); }


	private:
		InControl    mAmount;
	};

}

#endif // _CIRCULAR_SHIFT_H_

