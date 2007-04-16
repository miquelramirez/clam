/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
	typedef std::vector<MappingFactory*> MappingFactories;
	
	private:
		static MappingFactories& FactoryInstance();
	public:
		virtual TData Map(const TData in) = 0;
		static Mapping* Create(const std::string& name);
		virtual void Set(DataArray& arguments) { }
		virtual ~Mapping();
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

