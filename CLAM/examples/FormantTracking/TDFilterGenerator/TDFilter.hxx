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

#ifndef _TDFilter_
#define _TDFilter_

#include "Processing.hxx"
#include "Audio.hxx"
#include "TDFilterKernel.hxx"

namespace CLAM
{
	class TDFilterConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (TDFilterConfig, 2, ProcessingConfig);
		/** The processing name */
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** The audio input and output size **/
		DYN_ATTRIBUTE (1, public, TSize, AudioSize);
	protected:
		void DefaultInit()
		{
			// Instanciate all the attributes
			AddAll();
			UpdateData();
			SetAudioSize(0);
		}

	};

	class TDFilter: public Processing
	{
	public:
		TDFilter();

		TDFilter( TDFilterConfig &cfg );

		virtual ~TDFilter();

		const char *GetClassName() const { return "TDFilter"; }

		const ProcessingConfig &GetConfig(void) const { return mConfig; }

		bool Do(void) { return true; }

		bool Do( const Audio &x,
				 const TDFilterKernel &h, Audio &y );

	protected:

		bool ConcreteConfigure(const ProcessingConfig& c);

		bool ConcreteStart(void) { return true; }

		bool ConcreteStop(void) { return true; }

		inline void CheckTypes( const Audio &X,
								const Audio &Y ) const;

	private:

		TDFilterConfig		mConfig;

		TSize				mSize;
	};
} // namespace

#endif //TDFilter.hxx
