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


#ifndef __LPC__
#define __LPC__

#include "ProcessingComposite.hxx"
#include "Audio.hxx"
#include "AutocorrelationEvaluator.hxx"
#include "Levinson_Durbin.hxx"

namespace CLAM
{
	class LPCConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (LPCConfig,2,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,std::string,Name);
		DYN_ATTRIBUTE(1,public,int,Order);
	private:
		void DefaultInit();
	};

	class LPC: public ProcessingComposite
	{
	public:

		LPC();

		LPC( LPCConfig& cfg );

		~LPC();

		const char *GetClassName() const { return "LPC"; }

		const ProcessingConfig &GetConfig() const { return mConfig; }

		/** Supervised mode execution */
		bool Do(void) { return true; }

		/** Unsupervised mode execution */
		bool Do( const Audio& in, DataArray& A, DataArray& K, TData& E );

	protected:

		/** Configuration data */
		LPCConfig					mConfig;

		/** Number of LPC coefficients */
		int							mOrder;

		/** Processing Objects */
		AutocorrelationEvaluator	mAutocorrelationEval;
		Levinson_Durbin				mLevinson_Durbin;

		/** Internal convenience methods */
		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();
		bool ConcreteStart();
		bool ConcreteStop();

		/** Configuration method */
		bool ConcreteConfigure( const ProcessingConfig& );

	};

} //namespace

#endif
