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

#ifndef _AutomaticGainControl_
#define _AutomaticGainControl_

#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>

namespace CLAM{

	/**
	 *	AutomaticGainControl configuration object.
	 */
	class AutomaticGainControlConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( AutomaticGainControlConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, TData, AdaptationStep );

	protected:
		void DefaultInit();
	};

	/**	\brief Adaptative automatic gain control
	 *
	 *	Adaptative automatic gain control. Given an output reference
	 *	and step response adjusts the output volume to keep it constant.
	 */
	class AutomaticGainControl: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "AutomaticGainControl"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControl mOutputReference;

		TData gain;
		TData mStep; ///< adaptation step
		TData mORef; ///< output power reference

	public:
		AutomaticGainControl()
			:
			mInputAudio("Input Audio",this),
			mOutputAudio("Audio Output",this),

			mOutputReference("OutputReference",this)
		{
			Configure( mConfig );

			mOutputReference.SetBounds(0.,1.);
			mOutputReference.SetDefaultValue(0.1);
			mOutputReference.DoControl(0.1);

			gain = 1.; // starting gain
		}

 		~AutomaticGainControl() {}

		bool Do()
		{
			bool result = Do( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );

			mInputAudio.Consume(); 
			mOutputAudio.Produce();

			return result;
		}
	
		bool Do(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			mORef = mOutputReference.GetLastValue();
			for (int i=0;i<size;i++) 
			{
				outb[i] = inb[i] * gain;
				gain += mStep * ( mORef - outb[i]*outb[i] );
			}
			return true;
		}

		typedef AutomaticGainControlConfig Config;

	protected:

		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );
			mStep = mConfig.GetAdaptationStep();
			return true;
		}

	private:

		/** Configuration **/
		Config mConfig;
	};	
	
};//namespace CLAM

#endif // _AutomaticGainControl_

