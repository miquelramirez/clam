#ifndef __LOGMAGSPECTRUMVIEW__
#define __LOGMAGSPECTRUMVIEW__

#include "SpectrumAdapter.hxx"
#include "Array.hxx"

namespace CLAMVM
{

		class LogMagSpectrumAdapter 
				: public SpectrumAdapter
		{
				// attributes
		private:
				DataArray    mMagBuffer;
				DataArray    mPhaseBuffer;

		protected:
				void TrivialCaseConversion();
				void TranslateSpectralData();
		public:
				
				virtual ~LogMagSpectrumAdapter();

				virtual const char* GetClassName() const
				{
					return "LogMagSpectrumAdapter";
				}

				virtual bool Publish();
		};

}

#endif // LogMagSpectrumAdapter.hxx
