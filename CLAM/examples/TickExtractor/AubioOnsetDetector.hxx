#ifndef __AUBIOONSETDETECTOR__
#define __AUBIOONSETDETECTOR__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>
#include <aubio/aubio.h>
#include "Audio.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "Enum.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		class EODAlgorithms : public Enum
		{
		public:
			static tEnumValue  sEnumValues[];
			static tValue      sDefault;
			
			EODAlgorithms();
			EODAlgorithms( tValue val );
			EODAlgorithms( std::string s );

			typedef enum {
				eEnergy,
				eSpectralDifference,
				eHFC,
				eComplexDomain,
				ePhase
			};
			
			virtual Component* Species() const;
		};

		class AubioOnsetDetectorConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AubioOnsetDetectorConfig, 5, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData,         OnsetThreshold );
			DYN_ATTRIBUTE( 1, public, TData,         SilenceThreshold );
			DYN_ATTRIBUTE( 2, public, TSize,         WindowSize );
			DYN_ATTRIBUTE( 3, public, TSize,         HopSize );
			DYN_ATTRIBUTE( 4, public, EODAlgorithms, Method );

		protected:

			void DefaultInit();
		};

		class AubioOnsetDetector : public Processing
		{
		public:

			AubioOnsetDetector();
			~AubioOnsetDetector();

			const char* GetClassName() const;
			const ProcessingConfig& GetConfig() const;
			
			bool Do();
			bool Do( Audio& input, Array<TimeIndex>& onsets );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );
		private:

			AubioOnsetDetectorConfig mConfig;
			
			// aubio stuff
			// "Processings"
			aubio_pvoc_t*           mPhaseVocoder;
			aubio_onsetdetection_t*  mOnsetDetector;
			pickparams_t*           mPeakPickingParms;

			fvec_t*                     mCurrentFrame;
			fvec_t*                     mInputAudio;
			fvec_t*                     mOnsetBuffer;
			cvec_t*                     mSpectrum;			
			aubio_onsetdetection_type  mOnsetType;
			
		};

	}
       

}

#endif // AubioOnsetDetector.hxx
