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

#ifndef __FUNDFREQRM__
#define __FUNDFREQRM__

#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "Fl_Palette.hxx"
#include "GLRenderingManager.hxx"

namespace CLAMVM
{
	class FundFreqRM : public GLRenderingManager
	{
	public:
		FundFreqRM();
		virtual ~FundFreqRM();

		virtual void SetWorldBounds( const DataBoundBox& );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const CLAM::Array<TimeFreqMagPoint>& );
		virtual void RenderData();
		
	protected:

		virtual void ProcessData();
		virtual void DetermineVisibleSamples() {}
		virtual void Colorize();

	private:

		CLAM::Array<float>    mFreqBuffer;
		CLAM::Array<float>    mErrorBuffer;
		CLAM::Array<Fl_Color> mColorBuffer;
		Fl_Palette            mPalette;
		bool                  mMustProcessData;
		bool                  mMustColorize;
		DataBoundBox          mDataBBox;
		ScreenBoundBox        mScreenBBox;

	};
}

#endif // FundFreqRM.hxx
