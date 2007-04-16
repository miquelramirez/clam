#ifndef __STDIOSPECTRUMPRESENTATION__
#define __STDIOSPECTRUMPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv1;
		using CLAM::DataArray;
		using CLAM::TData;
		
		class SpectrumModel;

		class StdioSpectrumPresentation 
			: public Presentation
		{
				// attributes
		private:
				DataArray        mBinsMagnitude;
				DataArray        mBinsPhase;
				TData            mSpectralRange;


				// Implementation details
		protected:
				
				// callback methods to be called by the view
				virtual void OnNewMagBins( const DataArray& array );
				virtual void OnNewPhaseBins( const DataArray& array );
				virtual void OnNewSpecRng( TData specRange );

		public:
				// slots
				Slotv1<const DataArray&>         SetMagnitudeBins;
				Slotv1<const DataArray&>         SetPhaseBins;
				Slotv1<TData>                    SetSpectralRange;


				StdioSpectrumPresentation();

				virtual ~StdioSpectrumPresentation();

				virtual void AttachTo( SpectrumModel& );

				virtual void Detach();

				void Show();

				void Hide(); 

		};
		
}


#endif // StdioSpectrumPresentation.hxx

