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

#ifndef _WINDOW_GENERATOR_
#define _WINDOW_GENERATOR_

#include <typeinfo> // std::bad_cast
#include "DataTypes.hxx"
#include "GlobalEnums.hxx"
#include "Processing.hxx"
#include "InControl.hxx"
#include "Processing.hxx"
#include "WindowGeneratorConfig.hxx"
#include "Array.hxx"
#include "AudioOutPort.hxx"

namespace CLAM {

class Audio;
class Spectrum;
class Storage;
class ProcessingConfig;

/**
* This class can be used for generating Windows and for zeropadding the 
* windowed data. The Do functions accept different types of data,  which 
* are simple Arrays, Audio and Spectrum.
* <p>
* If the window is smaller as the Data Array then the rest of the Array 
* is filled with Zeroes.
* <p>
* The Size of the window can be changed via a control, the Type 
* (Hamming,Kaiser, etc) is fixed.
* A configuration option is to generate the window everytime  the  
* Do() function is called. This is settable via the UseTable element
* of the configuration.
*
* Note: The last and the first element of the window always have the
* same value, therefore, if you want to use it for windowing an FFT,
* you have to create a window of size FFTSize+1. (if you care for correctness)
*/

	/**
	 * This class can be used for generating Windows and for zeropadding the
	 * windowed data. The Do functions accept different types of data,  which
	 * are simple Arrays, Audio and Spectrum.
	 * <p>
	 * If the window is smaller as the Data Array then the rest of the Array
	 * is filled with Zeroes.
	 * <p>
	 * The Size of the window can be changed via a control, the Type
	 * (Hamming,Kaiser, etc) is fixed.
	 * A configuration option is to generate the window everytime  the
	 * Do() function is called. This is settable via the UseTable element
	 * of the configuration.
	 *
	 * Note: The last and the first element of the window always have the
	 * same value, therefore, if you want to use it for windowing an FFT,
	 * you have to create a window of size FFTSize+1. (if you care for correctness)
	 */

	class WindowGenerator: public Processing {
		WindowGeneratorConfig mConfig;

		const char *GetClassName() const {return "WindowGenerator";}

		/** Config change method
		 * @pre the argument should be an WindowGeneratorConfig object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:

		WindowGenerator();

		WindowGenerator(const WindowGeneratorConfig &c);

		~WindowGenerator();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(DataArray& output);
		bool Do(Spectrum& output);
		bool Do(Audio& output);

		// Port interfaces.

		bool SetPrototypes(const DataArray& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void SetSize(TSize size) 
		{
			CLAM_DEBUG_ASSERT(size%2==1,"Window size must be odd");
			mSize.DoControl((TControlData)size); 
		}

	protected:
		AudioOutPort  mOutput;

	private:
		InControl   mSize;
		DataArray   mTable;

		void CreateTable(DataArray& table,EWindowType windowType,long windowsize) const;
		void CreateWindowFromTable(DataArray &window) const;
		double BesselFunction(double x) const;
		void KaiserBessel(long size,DataArray& window,
		                  double alpha) const;
		void BlackmanHarrisX(long size,DataArray& window,
		                     double a0,double a1,double a2,double a3 = 0.) const;
		void BlackmanHarris62(long size,DataArray& window) const;
		void BlackmanHarris70(long size,DataArray& window) const;
		void BlackmanHarris74(long size,DataArray& window) const;
		void BlackmanHarris92(long size,DataArray& window) const;
		void BlackmanHarrisLike(long size,DataArray& window) const;
		void Hamming(long size,DataArray& window) const;
		void Triangular(long size,DataArray& window) const;
		void BlackmanHarris92TransMainLobe(long size,DataArray& window) const;
		void Gaussian(long size,DataArray& window) const;
	        void Sine(long size,DataArray& window) const;
	  
		void InvertWindow(const DataArray& originalWindow,
		                  DataArray& invertedWindow) const;
		void InvertWindow(DataArray& window) const;
		void NormalizeWindow(DataArray& window) const;
		double Sinc(double x, short N) const;

	};

};//namespace CLAM

#endif //_WINDOW_GENERATOR_

