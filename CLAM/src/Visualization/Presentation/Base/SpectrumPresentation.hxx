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

#ifndef __SPECTRUMPRESENTATION__
#define __SPECTRUMPRESENTATION__

// Includes of new Visualization Module
#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"

namespace CLAMVM
{
	using SigSlot::Slotv2;
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TTime;

	// forward declaration
	class SpectrumModel;
		
	/** \class SpectrumPresentation
	 *  \brief Abstract spectrum magnitude representation
	 *
	 *  This abstract class is the base class for all CLAM-based
	 *  spectrum magnitude graphical representations.
	 */
	class SpectrumPresentation : public Presentation
	{
				
	protected:

		/** \brief "Slotted function"
		 *
		 *  Slotted function that will catch the magnitude spectrum characteristic
		 *  attributes.
		 */
		virtual void OnNewSpectrum( const DataArray&, TData ) = 0;
	public:
    
		/** \brief Default constructor
		 *
		 *  Class default constructor
		 */
		SpectrumPresentation();
				
		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		~SpectrumPresentation();

		/** \brief Attaches object to an Spectrum Model
		 *
		 *  Attaches the presentation object to an Spectrum Model, so the object can be sent the
		 *  characteristics of the model object for rendering it.
		 *
		 *  \see SpectrumModel
		 */
		virtual void AttachTo( SpectrumModel& );

		/** \brief Detaches object from Spectrum Model
		 *
		 *   This method detaches the presentation from the Spectrum Model it is currently attached to.
		 *   If there is no currently Spectrum Model attached, then the method does nothing.
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

		/** \brief Sets the characteristics of the magnitude spectrum to display
		 *
		 *  This Slot receives two parameters:
		 *
		 *  \param magsamples  Spectrum magnitude samples
		 *  \param specrange   Spectral range
		 */
		Slotv2< const DataArray&, TData >       SetSpectrum;
				
	};
}


#endif // SpectrumPresentation.hxx
