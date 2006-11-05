/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef InstantTunningEstimator_hxx
#define InstantTunningEstimator_hxx
#include <list>
#include <vector>
#include <cmath>

namespace Simac
{


class InstantTunningEstimator
{
public:
	typedef std::vector<std::pair<double, double> > PeakList;
private:
	double _fasorX;
	double _fasorY;
	double _instantX;
	double _instantY;
	unsigned _binsPerSemitone;
	double _inertia;
public:
	InstantTunningEstimator(double inertia=0.0, unsigned binsPerSemitone=3)
		: _inertia(inertia)
		, _binsPerSemitone(binsPerSemitone)
	{
		_fasorX=1.0;
		_fasorY=0.0;
	}
	~InstantTunningEstimator()
	{
	}
	void setInertia(double inertia)
	{
		_inertia=inertia;
	}
	void doIt(unsigned int nPeaks, const double * peakPositions, const double * peakValues)
	{
		_fasorX*=_inertia;
		_fasorY*=_inertia;
		for (unsigned int peak=0; peak<nPeaks; peak++)
		{
			double radiantTunning=peakPositions[peak]*2*M_PI/_binsPerSemitone;
			_fasorX+=cos(radiantTunning)*peakValues[peak];
			_fasorY+=sin(radiantTunning)*peakValues[peak];
		}
	}
	// TODO: This function is taken by S&R of the previous one, no test!!
	void doIt(const std::vector<std::pair<double, double> >& peaks)
	{
		_fasorX*=_inertia;
		_fasorY*=_inertia;
		_instantX=0;
		_instantY=0;
		for (unsigned int peak=0; peak<peaks.size(); peak++)
		{
			double radiantTunning=peaks[peak].first*2*M_PI/_binsPerSemitone;
			_instantX+=cos(radiantTunning)*peaks[peak].second;
			_instantY+=sin(radiantTunning)*peaks[peak].second;
		}
		_fasorX += _instantX;
		_fasorY += _instantY;
	}
	std::pair<double,double> output() const
	{
		double tunning=std::atan2(_fasorY,_fasorX)*_binsPerSemitone/2/M_PI;
		double strength=std::sqrt(_fasorY*_fasorY+_fasorX*_fasorX);
		return std::make_pair(tunning, strength);
	}
	std::pair<double,double> instantTunning() const
	{
		double tunning=std::atan2(_instantY,_instantX)*_binsPerSemitone/2/M_PI;
		double strength=std::sqrt(_instantY*_instantY+_instantX*_instantX);
		return std::make_pair(tunning, strength);
	}
};

} // namespace Simac

#endif// InstantTunningEstimator_hxx

