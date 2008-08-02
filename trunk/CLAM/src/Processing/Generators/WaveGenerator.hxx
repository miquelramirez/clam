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

#ifndef _WAVE_GENERATOR_H
#define _WAVE_GENERATOR_H

#include "Audio.hxx"
#include "Processing.hxx"
#include "AudioOutPort.hxx"
#include "Enum.hxx"

namespace CLAM {

	class EWaveType : public Enum {
	public:
		
		EWaveType() : Enum(ValueTable(), eSine) {}
		EWaveType(tValue v) : Enum(ValueTable(), v) {};
		EWaveType(std::string s) : Enum(ValueTable(), s) {};
		
		typedef enum {
			eSine
		} tEnum;
		static tEnumValue * ValueTable()
		{
			static tEnumValue sEnumValues[] = {
				{EWaveType::eSine,"SineWave"},
				{0,NULL}
			};
			return sEnumValues;
		}

		virtual Component* Species() const
		{
			return (Component*) new EWaveType(eSine);
		};
	};


	template< typename WaveType > 
	class WaveFunctor {
	public:
		TData operator()(TTime x,TData amplitude);
	};

	class WaveGeneratorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (WaveGeneratorConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, EWaveType,   WaveType);
		DYN_ATTRIBUTE (1, public, TData,       Frequency);
		DYN_ATTRIBUTE (2, public, TData,       Amplitude);
		DYN_ATTRIBUTE (3, public, TData,       Phase);
		DYN_ATTRIBUTE (4, public, TData,       SampleRate);
		DYN_ATTRIBUTE (5, public, int,         FrameSize);
	protected:
		void DefaultInit(void);
	};

	class WaveGenerator: public Processing
	{
	protected:
		WaveGeneratorConfig mConfig;	
		
	public:
		// MRJ: Convenience type for template thingies to work
		class EWaveType_eSine
		{
		};
	private:

		TData mAmplitude;

		TData mXPos;    // Radians
		TData mXDelta;  // Radians

		EWaveType::tValue mType;

		const char *GetClassName() const {return "WaveGenerator";}

		/** Configuration change method
		 * @pre argument should be an WaveGeneratorConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		inline TData Sine(TTime pos);

	public:


		inline TData GetXPos() const
		{
			return mXPos;
		}

		inline void SetXPos( TData new_value )
		{
			mXPos = new_value;
		}

		inline TData GetXDelta() const
		{
			return mXDelta;
		}

		inline void SetXDelta( TData new_value )
		{
			mXDelta = new_value;
		}

		inline TData GetAmplitude() const
		{
			return mAmplitude;
		}

		OutPort<Audio> Output;

		WaveGenerator();

		WaveGenerator(const WaveGeneratorConfig &c);

		virtual ~WaveGenerator();

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

	private:

 	
	};

	template < typename WaveType >
	void FillBuffer(Array<TData> &buffer, WaveGenerator& generator, WaveType* dummy = 0 )
	{
		TData xvalue = generator.GetXPos(); 
		TData xdelta = generator.GetXDelta();
		TData amplitude = generator.GetAmplitude();


		int i;
		WaveFunctor<WaveType> func;
		int size = buffer.Size();
		for (i=0; i<size; i++) {
			buffer[i] = func( xvalue, amplitude);
			xvalue += xdelta;
		}
		if (xvalue > 2.0 * M_PI)
			xvalue = fmod(xvalue, TData( 2.0 * M_PI ));

		generator.SetXPos( xvalue );
	}


}//namespace CLAM

#endif // _WaveGenerator_

