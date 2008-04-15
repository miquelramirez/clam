/*
 * Copyright (c) 2007 by Hernan Ordiales <audiocode@uint8.com.ar>
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

#ifndef _GuitarDistortion_
#define _GuitarDistortion_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>

#include <CLAM/Audio.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControlArray.hxx>
#include <CLAM/Enum.hxx>

#include <CLAM/CLAM_Math.hxx>

namespace CLAM{

	static inline TData Sign( TData value )
	{
		return ( value > 0. ) ? 1. : -1.;
	}

	class EDistType : public Enum
	{
	public:
		EDistType() : Enum(ValueTable(), ePolynomial) {}
		EDistType(tValue v) : Enum(ValueTable(), v) {};
		EDistType(std::string s) : Enum(ValueTable(), s) {};
		virtual Component* Species() const { return new EDistType(ePolynomial); }
	
		typedef enum {
			eFactor,
			eAtan,
			ePolynomial
		};
	
		static tEnumValue * ValueTable()
		{
			static tEnumValue sValueTable[] =
			{
				{eFactor,"Factor"},
				{eAtan,"Atan"},
				{ePolynomial,"Polynomial"},
				{0,NULL}
			};
			return sValueTable;
		}
	};

	/**
	 *	GuitarDistortion configuration object.
	 */
	class GuitarDistortionConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( GuitarDistortionConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, EDistType, DistortionType );

	protected:
		void DefaultInit();
	};

	/**	\brief GuitarDistortion
	 *
	 *	overdriven guitar distortion
	 */
	class GuitarDistortion: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "GuitarDistortion"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControlArray mParams; ///< Parameters

		EDistType mDistType; ///< kind of distortion selector


		/** Factor distortion values **/
		const TData mCompMaxValue; ///< max compression default value
		TData mGain; ///< automatic mGain control
		TData mGainStep; ///< adaptation mGain step

	public:
		GuitarDistortion()
			:
			mInputAudio("Input Audio",this ),
			mOutputAudio("Audio Output",this),

			mParams(0,"Parameters", this),

			mCompMaxValue(10.)
		{
			Configure( mConfig );
		}

 		~GuitarDistortion() {}

		bool Do()
		{
			bool result = false;
			switch( mDistType )
			{
			case EDistType::eFactor:
				result = DoFactor( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );
				break;
			case EDistType::eAtan:
				result = DoAtan( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );
				break;
			case EDistType::ePolynomial:
				result = DoPolynomial( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );
				break;
			default:
				CLAM_ASSERT( false, "Bad mDistType" );
			}

			mInputAudio.Consume(); 
			mOutputAudio.Produce();

			return result;
		}
	
		/** \brief Factor distortion Do
		* 
		*	More interesting output in the 0.7-1.0 range.
		* 	It's better to work with an input with more resolution in the upper range
		*	or at least adjust the linear input like: amount=1.-CLAM_pow(1.-amount,4.);
		*	Uses a very simple compression.
		*/
		bool DoFactor(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			//Distortion param
			TData amount = mParams[0].GetLastValue();
			if (amount > 0.99) amount = 0.99; // to get it stable, avoids division by zero

			//Compression param
			TData omGainref = mParams[1].GetLastValue(); //output power reference
			mGainStep = 0.1;
			if (omGainref>mCompMaxValue-0.01)
				omGainref = mCompMaxValue-0.01;
			else if (omGainref<0.01) 
			{
				//compression off
				mGainStep = 0.;
				mGain = 1.;
			}
			omGainref = (mCompMaxValue-omGainref)/100.; //adjust and inverts the slider value

			TData k = 2. * amount / ( 1. - amount );
			for (int i=0;i<size;i++) 
			{
				outb[i] = (1.+k)*inb[i] / ( 1. + k*Abs(inb[i]) ) * mGain;
				mGain += mGainStep * ( omGainref - outb[i]*outb[i] );
			}
			return true;
		}

		/** \brief Atan distortion Do
		* 
		*	Atan based distortion
		*/
		bool DoAtan(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			TData amount = mParams[0].GetLastValue();
// 			amount -= 50;
			if (amount==0.) amount = 0.01; // avoids atan(0)=0 and further zero division
			for (int i=0;i<size;i++) 
			{
				outb[i] = CLAM_atan( inb[i]*amount ) / CLAM_atan(amount);
			}
			return true;
		}

		/** \brief Polynomial distortion
		* 
		*/
		bool DoPolynomial(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			TData a = mParams[0].GetLastValue();
			TData b = mParams[1].GetLastValue();

			for (int i=0;i<size;i++) 
			{
				outb[i] = a*inb[i] + b*( inb[i]*inb[i] )*Sign(inb[i]);
			}
			return true;
		}

		typedef GuitarDistortionConfig Config;


	protected:

		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );

			unsigned int controls_amount = 0;

			mDistType = mConfig.GetDistortionType();
			switch( mDistType )
			{
			case EDistType::eFactor:
				controls_amount = 2;
				break;
			case EDistType::eAtan:
				controls_amount = 1;
				break;
			case EDistType::ePolynomial:
				controls_amount = 2;
				break;
			default:
				CLAM_ASSERT( false, "Bad mDistType" );
			}

			mParams.Resize(controls_amount, "Param", this);
			for (int i=0; i < mParams.Size(); i++)
			{
				mParams[i].SetBounds(0.,1.);
				mParams[i].SetDefaultValue(0.);
				mParams[i].DoControl(0.);
			}

			switch( mDistType )
			{
			case EDistType::eFactor:
				//Compression
				mParams[1].SetBounds(0.,mCompMaxValue);
				mParams[1].SetDefaultValue(mCompMaxValue*0.9);
				mParams[1].DoControl(mCompMaxValue*0.9);

				mGainStep = 0.1; //fixed default value
				mGain = 1.; // starting mGain
				break;

			case EDistType::eAtan:
				mParams[0].SetBounds(0,100);
				mParams[0].SetDefaultValue(0.5);
				mParams[0].DoControl(0.5);
				break;

			case EDistType::ePolynomial:
				mParams[0].SetBounds(0,100);
				mParams[0].SetDefaultValue(1);
				mParams[0].DoControl(1);

				mParams[1].SetBounds(0,100);
				mParams[1].SetDefaultValue(0);
				mParams[1].DoControl(0);
				break;
			default:
				CLAM_ASSERT( false, "Bad mDistType" );
			}

			return true;
		}

	private:

		/** Configuration **/
		Config mConfig;
	};	
	
};//namespace CLAM

#endif // _GuitarDistortion_
