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

#ifndef _SOFT_SWITCH_HXX
#define _SOFT_SWITCH_HXX

#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include "Audio.hxx"
#include "Enum.hxx"

namespace CLAM
{

	class EFadeType : public Enum {
	public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EFadeType() : Enum(sEnumValues, sDefault) {}
		EFadeType(tValue v) : Enum(sEnumValues, v) {};
		EFadeType(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eSharp,
			eLinear,
			eExponential,
		};
		
		virtual Component* Species() const
		{
			return (Component*) new EFadeType(eLinear);
		};
	};


	class SoftSwitchConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SoftSwitchConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TData,       SampleRate);
		DYN_ATTRIBUTE (2, public, int,         FrameSize);
		DYN_ATTRIBUTE (3, public, TTime,       TransitionTime);
		DYN_ATTRIBUTE (4, public, EFadeType,   FadeType);
		DYN_ATTRIBUTE (5, public, bool,        EnabledAtStart);

	protected:

		void DefaultInit(void);
	};

	/**
	 * This class implements an audio switch. Transitions from
	 * silence output to bypass and viceversa are "soft", i.e,
	 * configurable fadeout and attack ramps are used.
	 */
	class SoftSwitch: public Processing
	{
	public:

		InPortTmpl<Audio>  Input;
		OutPortTmpl<Audio> Output;

		/** Switch control
		 *  0.0 value      -> Output is enabled.
		 *  non-zero value -> Output is set to silence.
		 */
		InControl          Enable;

	private:

		typedef enum {
			eOn,
			eOff,
			eAttack,
			eFadeout,
		} ESwitchState;

		ESwitchState mState;
		TTime mTransitionPos;
		TTime mTransitionLength;
		TData mTransitionInitValue;
		TData mTransitionCurrentValue;
		TData mDelta;

		SoftSwitchConfig  mConfig;
		EFadeType::tValue mType;
		int               mFrameSize;

		void Attack(const Array<TData> &inp, Array<TData> &outp);

		void Fadeout(const Array<TData> &inp, Array<TData> &outp);

		void Silence(Array<TData> &outp);

		template<class FADER>
		void FadeoutTmpl(const Array<TData> &inp, Array<TData> &outp);

		template<class FADER>
		void AttackTmpl(const Array<TData> &inp, Array<TData> &outp);

		void CheckControl();

		bool ConcreteConfigure(const ProcessingConfig& c);

		bool ConcreteStart();

	public:

		SoftSwitch(const SoftSwitchConfig& c = SoftSwitchConfig());

		const char* GetClassName() const {return "SoftSwitch";}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Audio& inp, Audio& out);

		void StoreOn(Storage &s) {};
	};
	
}

#endif
