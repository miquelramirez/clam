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

#ifndef __Lenvinson_Durbin__
#define __Lenvinson_Durbin__

#include "Processing.hxx"
//#include "Correlation.hxx"
#include "Audio.hxx"
#include "Array.hxx"

namespace CLAM
{
	class Levinson_DurbinConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Levinson_DurbinConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int, Order);
	protected:
		/** Dynamic type initialization: All attributes are
		 *   instantiated.
		 */
		void DefaultInit(void);		
	};

	class Levinson_Durbin: public Processing
	{
	protected:
		/** Levinson_Durbin Configuration */
		Levinson_DurbinConfig mConfig;
		/** Order Filter */
		int mOrder;

		const char *GetClassName() const { return "Levinson_Durbin"; }

		int GetOrder() { return mOrder; }

		inline void CheckTypes( /*const Correlation& in*/Audio& in, const DataArray& A, const DataArray& K ) const;

		//inline void CalculateK( TData& Ki, const DataArrray& A, const Correlation& R, const TData& E, const int& i  );

		//inline void CalculateA( DataArray& A, const DataArrray& K, const int& i  );

		bool ConcreteConfigure( const ProcessingConfig& );

		bool ConcreteStart();

		bool ConcreteStop();

	public:

		Levinson_Durbin();

		Levinson_Durbin( const Levinson_DurbinConfig& cfg );

		virtual ~Levinson_Durbin();

		/** Configuration access:
		*/
		const ProcessingConfig &GetConfig() const { return mConfig; }

		/** Supervised-mode Do function.
		*/

		bool Do(void) { return true; }

		/** Standard Levinson-Durbin Do function
		*/

		bool Do( /*const Correlation& in*/Audio& in, DataArray& A, DataArray& K, TData& E );

	};
}

#endif
