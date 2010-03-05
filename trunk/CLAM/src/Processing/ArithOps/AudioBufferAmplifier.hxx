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

#ifndef _AudioBufferAmplifier_
#define _AudioBufferAmplifier_

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Audio.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"

namespace CLAM{

	/**
	 *	AudioBufferAmplifier configuration object.
	 */
	class AudioAmplifierConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( AudioAmplifierConfig, 3, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, double, MaxGain );
		DYN_ATTRIBUTE( 1, public, double, InitGain);
		DYN_ATTRIBUTE( 2, public, int, PortsNumber);

	protected:
		void DefaultInit();
	};


	/**	\brief Time-domain audio gain
	 *
	 *	Very simple time-domain linear audio amplifier. For lower the volume choose values between 0 and 1.
	 *  Values over 1 will amplify the audio signal. Gain=1 means no change.
	 */
	class AudioBufferAmplifier: public Processing
	{
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "AudioBufferAmplifier"; }

		/** Ports **/
		std::vector<InPort<Audio>*> _inputs;
		std::vector<OutPort<Audio>*> _outputs;

		/** Controls **/
		FloatInControl _gainControl; ///< gain control

	public:
		AudioBufferAmplifier(const ProcessingConfig & config=Config())
			: _gainControl("Gain", this)
		{
			Configure(config);
		}

 		~AudioBufferAmplifier()
		{
			ResizePorts(0);
		}

		bool Do()
		{
			bool result=true;
			TData gain = _gainControl.GetLastValue();
			for (unsigned i=0; i<_inputs.size(); i++)
			{
				result &= Do( _inputs[i]->GetData(), _outputs[i]->GetData(), gain );
				_inputs[i]->Consume();
				_outputs[i]->Produce();
			}
			_previousGain = gain;
			return result;
		}

		bool Do(const Audio& in, Audio& out, float gain)
		{
			int size = in.GetSize();
			out.SetSize(size);
			const TData * inb = in.GetBuffer().GetPtr();
			TData * outb = out.GetBuffer().GetPtr();
			TData gainDelta=(gain-_previousGain)/size;
			TData rampedGain = _previousGain;

			for (int i=0;i<size;i++)
			{
				outb[i] = inb[i]*rampedGain;
				rampedGain+=gainDelta;
			}
			return true;
		}

		typedef AudioAmplifierConfig Config;

        virtual bool SupportsVariableAudioSize() const {return true;}

	protected:
		const ProcessingConfig& GetConfig() const { return _config; }
		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( _config, config );
			double max_gain = _config.GetMaxGain();
			double init_gain = _config.HasInitGain() ? _config.GetInitGain() : 1. ;
			_gainControl.SetBounds(0.,max_gain);
			_gainControl.SetDefaultValue(init_gain);
			_gainControl.DoControl(init_gain);
			_previousGain = init_gain;
			ResizePorts(_config.HasPortsNumber()?_config.GetPortsNumber():1);
			return true;
		}
	private:
		void ResizePorts(unsigned newSize);

		Config _config;
		TData _previousGain;
	};

};//namespace CLAM

#endif // AudioBufferAmplifier

