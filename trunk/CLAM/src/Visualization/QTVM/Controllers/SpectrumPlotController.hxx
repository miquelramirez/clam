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

#ifndef __SPECTRUMPLOTCONTROLLER__
#define __SPECTRUMPLOTCONTROLLER__

#include "Spectrum.hxx"
#include "DataRenderer.hxx"
#include "SelPosPlotController.hxx"
namespace CLAM
{
    namespace VM
    {
	class SpectrumPlotController : public SelPosPlotController
	{
	    Q_OBJECT

	public:
	    SpectrumPlotController();
	    virtual ~SpectrumPlotController();

	    void SetData(const Spectrum& spec);
	    void SetDataColor(Color c);
	    void SurfaceDimensions(int w,int h);

	    virtual void Draw();
	    void SetMousePos(TData x,TData y);

	    virtual void SetSelPos(const TData& value);

	signals:
	    void mag(TData);
	    void freq(TData);

	protected:
	    virtual void SetHBounds(const TData& left,const TData& right);				
	    virtual void SetVBounds(const TData& bottom,const TData& top);
	    void FullView();				
	    TData GetSpectralRange() const;				
	    bool MustProcessData() const;

	private:
	    Spectrum  _spec;
	    DataRenderer _renderer;
	    DataArray _magBuffer;
	    DataArray _phaseBuffer;
	    DataArray _processedData;

	    bool _mustProcessData;
	    TData _spectralRange;

	    void AdaptSpectralData();
	    void CacheData();
	    void ProcessData();
	    int GetXMaxMajStep(int ref) const;				
	    int GetYMaxMajStep(int ref) const;

	};
    }
}

#endif


