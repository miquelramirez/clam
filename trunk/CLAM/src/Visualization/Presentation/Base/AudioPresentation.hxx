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
#ifndef __AUDIOPRESENTATION__
#define __AUDIOPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv4.hxx"

namespace CLAMVM
{
	using SigSlot::Slotv4;
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TTime;

	// forward declaration
	class AudioModel;

	/**  \class AudioPresentation
	 *   \brief Abstract Audio signal concept representation			
	 *
	 *   This abstract class is the base class for all CLAM-based
	 *   audio signal graphical representations.
	 */
	class AudioPresentation 
		: public Presentation
	{
	protected:				
		/** \brief "Slotted method"
		 *
		 *  Slotted function that will catch the audio signal characteristic
		 *  attributes.
		 */
		virtual void OnNewAudio( const DataArray& array, TTime start, TTime duration, TData  srate ) = 0;				
	public:

		/** \brief Default constructor
		 *
		 *  Class default constructor
		 */
		AudioPresentation();

		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		~AudioPresentation();

		/** \brief Attaches object to an AudioModel
		 *
		 *  Attaches the presentation object to an AudioModel, so the object can be sent the 
		 *  characteristics of the model object for rendering it.
		 *
		 *  \see AudioModel
		 */
		virtual void AttachTo( AudioModel&  );

		/** \brief Detaches object from AudioModel
		 *
		 *  This method detaches the presentation from the AudioModel it is currently attached to. If
		 *  the presentation is not attached to anybody, then this method does nothing.
		 *
		 *  \see AudioModel
		 */
		virtual void Detach();

		/** \brief Inherited from CLAMVM::Presentation
		 *
		 *  \see Presentation::Show
		 */
		virtual void Show() = 0;
				
		/** \brief Inherited from CLAMVM::Presentation
		 *
		 *  \see Presentation::Hide
		 */
		virtual void Hide() = 0;

		/** \brief Sets the characteristis of the audio signal to display
		 *
		 *  This is an Slot that receives four parameters ( as can be seen in its signature, expressed
		 *  as template parameters ).
		 *  The parameters, listed in order are:
		 *
		 *  \param samples  Audio signal samples
		 *  \param start    Audio signal start time ( in secs )
		 *  \param duration Audio signal total duration
		 *  \param srate    Audio signal sample rate
		 */
		Slotv4<const DataArray&, TTime, TTime, TData> SetAudio;
	};
}

#endif // AudioPresentation.hxx
