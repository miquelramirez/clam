/*
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

#ifndef HistogramViewMonitor_hxx
#define HistogramViewMonitor_hxx

#include "FloatArrayDataSource.hxx"
#include <CLAM/PortMonitor.hxx>

#include <vector>

/**	\brief Histogram view
*
*	Useful to plot noise distributions
*/
class HistogramViewMonitor :
	public CLAM::AudioPortMonitor,
	public CLAM::VM::FloatArrayDataSource
{
public:
	HistogramViewMonitor()
		: _size(0), _bins(512)
	{
		_data.resize( _bins );
		for(unsigned int i=0;i<_data.size();i++)
			_data[i] = 0.;
	}
private:
	const char* GetClassName() const { return "HistogramView"; };

	std::string getLabel(unsigned bin) const
	{
		static std::string dummyLabel;
		return dummyLabel;
	}
	const CLAM::TData * frameData()
	{
		const CLAM::Audio &audio = FreezeAndGetData();
		const CLAM::Array<CLAM::TData> &inb = audio.GetBuffer();
		_size = inb.Size();
 		if (_size==0) return 0;

		for (int i=0; i<audio.GetSize(); i++ )
		{
			CLAM_ASSERT(fabs(inb[i])<=1., "Input audio is out of range (-1..1)");

			//index calculation by integer division
			unsigned int index = (inb[i]+1)*32768/(65536/_bins); // if bins=512 and maxValue=65536 => max index 65536/128 = 512 (number of bins)
			_data[ index ]++;
		}

		return &_data[0];
	}
	void release()
	{
		UnfreezeData();
	}
	unsigned nBins() const
	{
		return _bins;
	}
	virtual bool hasUpperBound() const { return false; }
	virtual bool hasLowerBound() const { return false; }
	virtual CLAM::TData upperBound() const {return 1;}
	virtual CLAM::TData lowerBound() const {return 0;}
	bool isEnabled() const
	{
		return IsRunning();
	}
private:
	unsigned _size;
	const unsigned _bins; //fixed number of bins. TODO: Add this as a configuration option.
	std::vector<CLAM::TData> _data; ///< Data of the histogram
};


#endif// HistogramViewMonitor_hxx
