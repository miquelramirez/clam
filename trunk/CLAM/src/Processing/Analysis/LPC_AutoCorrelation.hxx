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
#ifndef __LPC_AUTOCORRELATION__
#define __LPC_AUTOCORRELATION__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "LPModel.hxx"

namespace CLAM
{
	class Audio;
	template < class T > class Array;

	class LPCConfig
		: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( LPCConfig, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, std::string, Name  );
		/**
		 *  The order of the polynomial used to fit the analyzed signal spectrum
		 */
		DYN_ATTRIBUTE( 1, public, unsigned,       Order );
	protected:
		void DefaultInit();
	};

	class LPC_AutoCorrelation 
		: public Processing
	{
	public:
		LPC_AutoCorrelation();
		virtual ~LPC_AutoCorrelation();

		const char* GetClassName() const;

		const ProcessingConfig& GetConfig() const;

		bool  Do(); 

		bool  Do( const Audio& in, LPModel& out );

		bool  Do( const Audio& in, DataArray& A, DataArray& K, TData& E );

	protected:

		LPCConfig            mCurrentConfig;
		
		bool ConcreteConfigure( const ProcessingConfig& cfg );

		void ComputeAutocorrelation( const Array<TData>& signal,
					     Array<TData>& acCoeffs );

		void SolveSystemByLevinsonDurbin( const Array<TData>& R,
						  Array<TData>& A,
						  Array<TData>& K,
						  TData&        E);

	private:
	};

	// inlines

	inline const char* LPC_AutoCorrelation::GetClassName() const
	{
		return "LPC_Autocorrelation"; 
	}

	inline const ProcessingConfig& LPC_AutoCorrelation::GetConfig() const
	{
		return mCurrentConfig;
	}

}

#endif // LPC_AutoCorrelation.hxx
