#ifndef __SPECTRALPEAKSPRESENTATION__
#define __SPECTRALPEAKSPRESENTATION__

#include "Presentation.hxx"
#include "Partial.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
	using SigSlot::Slotv1;
	using SigSlot::Slotv2;
	using CLAM::Array;
	using CLAM::DataArray;
	using CLAM::TData;
	
	// forward declarations
	class SpectralPeaksModel;
	class SpectrumModel;
	
	/** \class SpectrumPlusPeaksPresentation
	 *  \brief Abstract class for representations of magnitude spectrum and corresponding peaks
	 *
	 *  This abstract class is the base class for all CLAM-based representations
	 *  a magnitude spectrum and the peaks detected in it.
	 */
	class SpectrumPlusPeaksPresentation : public Presentation
	{
	protected:
		/** \brief "Slotted method"
		 *
		 *  Slotted function that will catch the magnitude spectrum characteristic
		 *  attributes.
		 */
		virtual void OnNewSpectrum( const DataArray&, TData ) = 0; 
		
		/** \brief "Slotted method"
		 *
		 *  Slotted function that will catch the set of partials for the observed
		 *  spectrum
		 */
		virtual void OnNewPeakArray( const Array<Partial>& ) = 0;
		
	public:
		
		/** \brief Constructor
		 *  
		 *  Default constructor
		 */
		SpectrumPlusPeaksPresentation();
		
		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		~SpectrumPlusPeaksPresentation();
		
		/** \brief Attaches object to an Spectrum Model and a Spectral Peaks Model
		 *
		 *  Attaches the presentation object to both Spectrum Model and a Spectral Peaks
		 *  Model, so they can represented together. It is responsability of the application
		 *  to provide the matching pair of models.
		 */
		virtual void AttachTo( SpectrumModel& , SpectralPeaksModel& );

		/** \brief Detaches object from attached Models
		 *
		 *  If the presentation object is attached to some Model this method detaches it. Otherwise
		 *  this method does nothing.
		 */
		virtual void Detach();
		
		/** \brief Inherited from CLAMVM::Presentation
		 *
		 *  \see Presentation::Show
		 */
		virtual void Show() = 0;

		/** \brief Inherited from CLAMVM::Hide
		 * 
		 *  \see Presentation::Hide
		 */
		virtual void Hide() = 0;
		
		/** \brief Sets the caracteristics of the magnitude spectrum to be displayed
		 *
		 *  This slot receives two parameters:
		 *  \param magsamples  Spectrum magnitude samples
		 *  \param specrange   Spectral range
		 */
		Slotv2< const DataArray&, TData >   SetSpectrum;

		/** \brief Sets the set of partials to be displayed
		 *
		 *  This Slot just receives just one parameter:
		 *  \param partialset Set of partials to be displayed
		 *
		 *  \sa Array, Partial
		 */
		Slotv1< const Array<Partial>& >     SetPartials;
	};
}

#endif // SpectrumPlusPeaksPresentation.hxx
