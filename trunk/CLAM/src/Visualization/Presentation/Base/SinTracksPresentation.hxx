#ifndef __SINTRACKSPRESENTATION__
#define __SINTRACKSPRESENTATION__

#include "Presentation.hxx"
#include "Partial.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"
#include "Slotv1.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
	using SigSlot::Slotv1;
	using SigSlot::Slotv2;
	using CLAM::Array;
	using CLAM::TData;
	using CLAM::TTime;
	using CLAM::TSize;

	// forward declaration
	class SinTracksModel;
	
	/** \class SinTracksPresentation
	 *  \brief Abstract representation for Sinusoidal Tracks
	 *
	 *  This abstract class is the base class for all CLAM-based sinusoidal tracks
	 *  graphical representations
	 */
	class SinTracksPresentation : public Presentation
	{
	protected:
		
		/** \brief "Slotted function"
		 *
		 *  Slotted method that will catch provided Sinusoidal Tracks, as well 
		 *  as the number of frames that they span
		 */
		virtual void OnNewTrackList( SineTrackList& , TSize ) = 0; 
				
		/** \brief "Slotted function"
		 *
		 *  Slotted method that will catch provided Spectral ranges so drawing
		 *  them up-to-scale in the frequency axis.
		 */
		virtual void OnNewRange( TData ) = 0;

		/** \brief "Slotted function"
		 *
		 *  Slotted method that will catch provided start and end times for the
		 *  sinusoidal tracks received. This allows to draw up-to-scale the tracks
		 *  up-to-scale in the time axis.
		 */
		virtual void OnNewDuration( TTime begin, TTime end ) = 0;
	public:

		/** \brief Constructor
		 *
		 *  Default constructor
		 */
		SinTracksPresentation();

		/** \brief Destructor
		 *
		 *  Class destructor.
		 */
		~SinTracksPresentation();

		/** \brief Attaches object to an sinusoidal tracks model
		 *
		 *  Attaches the presentation object to a model, that will provide the sinusoidal
		 *  tracks data for being rendered.
		 * 
		 *  \see SinTracksModel
		 */
		virtual void AttachTo( SinTracksModel& );

		/** \brief Detaches object from attached model
		 *
		 *  This method detaches the object from the attached Model. If the presentation is not
		 *  attached to any model then the method does nothing.
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

		/** \brief Sets the sinusoidal tracks list to be rendered
		 *
		 *  Take this Slot as equivalent to the following function:
		 *    void SetTrackList( SineTrackList& list, TSize spannedframes )
		 *
		 *  being its parameters:
		 *  \param list          The sinusoidal track list to be rendered
		 *  \param spannedframes The list frame span
		 *  \sa SineTrackList
		 */
		Slotv2<SineTrackList&, TSize >          SetTrackList;
		
		/** \brief Sets the spectral range
		 * 
		 *  Take this Slot as equivalent to the following function:
		 *          void SetSpectralRange( TData specrange );
		 *
		 *  \param specRange  The spectral Range ( in Hz )
		 */
		Slotv1<TData>                           SetSpectralRange;

		/** \brief Sets the time interval
		 *
		 *  Take this Slot as equivalent to the following function:
		 *           void SetTimeInterval( TTime start_time, TTime end_time )
		 *
		 *  \param start_time  Time in seconds for the start of sinusoidal tracks list
		 *  \param end_time    Time in seconds for the end of the sinusoidal track list
		 */
		Slotv2<TTime, TTime>                    SetTimeInterval;
	};
}

#endif // SinTracksPresentation.hxx
