#ifndef _Mapping_
#define _Mapping_

#include "CLAM_Math.hxx"
#include <string>
#include <vector>
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAM {

	class MappingFactory;

	class Mapping
	{
	friend class MappingFactory;
	private:
		static std::vector<MappingFactory*> sFactories;
	public:
		virtual TData Map(const TData in) = 0;
		static Mapping* Create(const std::string& name);
		virtual void Set(DataArray& arguments) { }
	};

	class MappingFactory
	{
	friend class Mapping;
	private:
		std::string mName;
	public:
		MappingFactory(const std::string& name)
		:mName(name)
		{
		}

		void AddMe(void);

		virtual Mapping* Create(void) = 0;

		virtual ~MappingFactory()
		{
		}
	};


	class ValueToRatioMapping:public Mapping
	{
	private:
		TData mSemitones ;
	public:
		ValueToRatioMapping()
		{
		}

		void Set( const TData& inSemitones ) 
		{
			mSemitones = inSemitones ;
		}

		TData Map( const TData inValue )
		{
			return ( TData(pow( 2. , ( ( mSemitones / 12. ) * ( ( inValue - 8192. ) / 8192. ) ) )) ) ;
		}

		void Set(DataArray& arguments)
		{
			CLAM_ASSERT( arguments.Size() == 1,
				"Not enough arguments for ValueToRatioMapping" );
			Set( arguments[0] ) ;
		}
	};

	class ValueToRatioMappingFactory:public MappingFactory
	{
	public:
		static ValueToRatioMappingFactory sSingleton ;
		ValueToRatioMappingFactory():MappingFactory( "ValueToRatio" )
		{
			AddMe() ;
		}
		Mapping* Create(void) { return new ValueToRatioMapping; }
	};



	class NoteToFreqMapping:public Mapping
	{
	private:
		TData mNoteRef ;
		TData mFrequencyRef ;
	public:
		NoteToFreqMapping()
		{
		}

		void Set( 
			const TData& inNoteRef,
			const TData& inFrequencyRef    )
		{
			mNoteRef = inNoteRef ;
			mFrequencyRef = inFrequencyRef ;
		}

		TData Map( const TData inNote )
		{
			return ( TData( pow( 2.0 , ( inNote - mNoteRef ) / 12.0 ) ) ) * mFrequencyRef ;
		}

		void Set(DataArray& arguments)
		{
			if ( arguments.Size() == 0 )
			{
				// no arguments have been specified, so use the default values:
				// reference a 440 Hz at midi note 69
				Set(69,440);
			}else
			{
				CLAM_ASSERT(arguments.Size() == 2,
					"Not enough arguments for NoteToFreqMapping" );
				Set( arguments[0], arguments[1] ) ;
			}
		}
	};

	class NoteToFreqMappingFactory:public MappingFactory
	{
	public:
		static NoteToFreqMappingFactory sSingleton ;
		NoteToFreqMappingFactory():MappingFactory( "NoteToFreq" )
		{
			AddMe() ;
		}
		Mapping* Create(void) { return new NoteToFreqMapping; }
	};

	class LinearMapping:public Mapping
	{
	private:
		TData mInOffset,mOutOffset,mScale;
	public:
		LinearMapping()
		{
			mInOffset = 0;
			mOutOffset = 0;
			mScale = 1;
		}

		void Set(
			const TData& inA,const TData& inB,
			const TData& outA,const TData& outB)
		{
			mInOffset = inA;
			mOutOffset = outA;
			mScale = (outB-outA)/(inB-inA);
		}
		TData Map(const TData in)
		{
			return mOutOffset+(in-mInOffset)*mScale;
		}

		void Set(DataArray& arguments)
		{
			CLAM_ASSERT(arguments.Size()==4,
				"Not enough arguments for LinearMapping");
			Set(arguments[0],arguments[1],arguments[2],arguments[3]);
		}

	};

	class LinearMappingFactory:public MappingFactory
	{
	public:
		static LinearMappingFactory sSingleton;
		LinearMappingFactory():MappingFactory("linear")
		{
			AddMe();
		}
		Mapping* Create(void) { return new LinearMapping; }
	};


}

#endif
