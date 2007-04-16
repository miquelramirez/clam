#ifndef __STDIOSPECTRALPEAKARRAYPRESENTATION__
#define __STDIOSPECTRALPEAKARRAYPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"


namespace CLAMVM
{
		using SigSlot::Slotv1;
		using CLAM::Array;
		
		class SpectralPeaksModel;

		class StdioSpectralPeakArrayPresentation : public Presentation
		{
		private:
				Array<Partial>         mPartialsToDraw;


		protected:


				// callback methods to be called by the view

				virtual void OnNewPartials( const Array<Partial>& array );
		public:

				Slotv1<const Array<Partial>& >            SetPartials;

				StdioSpectralPeakArrayPresentation();
				virtual ~StdioSpectralPeakArrayPresentation();
				
				virtual void AttachTo( SpectralPeaksModel& );
				virtual void Detach();

				void Show();
				void Hide();
		};	
}

#endif // StdioSpectralPeakArrayPresentation.hxx

