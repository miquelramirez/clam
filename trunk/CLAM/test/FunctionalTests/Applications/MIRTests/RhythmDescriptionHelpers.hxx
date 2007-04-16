#ifndef __RHYTHMDESCRIPTIONHELPERS__
#define __RHYTHMDESCRIPTIONHELPERS__

#include "DynamicType.hxx"
#include "Text.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Pulse.hxx"
#include "Audio.hxx"

namespace CLAMTest
{
	namespace RhythmDescriptionTests
	{

		class RhythmEventsB2B
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( RhythmEventsB2B, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,      RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::Pulse,     Events );

		protected:
			void DefaultInit();
			
		public:
			
			bool IsValid() const;
		};

		class TransientEventList
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( TransientEventList, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,                     RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::Array< CLAM::TimeIndex >, Events );

		protected:

			void DefaultInit();

		public:

			bool IsValid() const;
		};

		class IOIHistogramB2B
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( IOIHistogramB2B, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,  RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::Audio, IOIHistogram );
			
		protected:
			
			void DefaultInit();

		public:

			bool IsValid() const;
		};

		class TickSequence
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( TickSequence, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,      RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::DataArray, TickSequence );
		protected:

			void DefaultInit();
		};

		class EventListDifference :
			public CLAM::DynamicType
		{
			DYNAMIC_TYPE( EventListDifference, 9 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,                     FileTested );
			DYN_ATTRIBUTE( 1, public, bool,                           LengthMatched );
			DYN_ATTRIBUTE( 2, public, CLAM::Array< CLAM::TimeIndex >, Difference );
			DYN_ATTRIBUTE( 3, public, CLAM::TData,                    AveragePositionDeviation );
			DYN_ATTRIBUTE( 4, public, CLAM::TData,                    AverageWeightDeviation );
			DYN_ATTRIBUTE( 5, public, CLAM::TData,                    MinTimePositionDeviation );
			DYN_ATTRIBUTE( 6, public, CLAM::TData,                    MaxTimePositionDeviation );
			DYN_ATTRIBUTE( 7, public, CLAM::TData,                    MinWeightDeviation );
			DYN_ATTRIBUTE( 8, public, CLAM::TData,                    MaxWeightDeviation );

		protected:

			void DefaultInit();

		public:
			
			void Compare( const CLAM::Array< CLAM::TimeIndex >& truth,
				      const CLAM::Array< CLAM::TimeIndex >& yield );

			void CompareWithIdeal( const TickSequence& ideal,
					       const CLAM::Array< CLAM::TimeIndex >& yield );
			
		};

	}
}

#endif // RhythmDescriptionHelpers.hxx

