/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _RD_AUTOCORRELATIONTD_
#define _RD_AUTOCORRELATIONTD_

#include <CLAM/Processing.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/DataTypes.hxx>

namespace CLAM
{

	namespace RhythmDescription
	{

		class AutoCorrelationTDConfig : public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (AutoCorrelationTDConfig, 3,ProcessingConfig);
			DYN_ATTRIBUTE (0, public,int , UpperLimit);
			DYN_ATTRIBUTE (1, public,bool , AutomaticIntegTime);
			DYN_ATTRIBUTE (2, public,int , IntegrationTime);

			void DefaultInit();	
		};

		class AutoCorrelationTD : public Processing
		{
		public:
			AutoCorrelationTD();

			~AutoCorrelationTD();

			const char *GetClassName() const;
			bool Do(void);
			bool Do(Array<TData>& sequence, Array<TData>& acf);
			const ProcessingConfig &GetConfig() const;

		protected:

			bool ConcreteConfigure(const ProcessingConfig&);

		private:			
			AutoCorrelationTDConfig mConfig;

		};

	}

} // namespace CLAM

#endif // RD_AutoCorrelationTD.hxx

