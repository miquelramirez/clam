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

#ifndef _StatisticalProperties_
#define _StatisticalProperties_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/CLAM_Math.hxx>

namespace CLAM {

	/**	\brief Statistical properties
	*
	*	Mean, Standard deviation, min, max, etc.
	*/
	class StatisticalProperties: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "StatisticalProperties"; }

		/** Ports **/
		AudioInPort mInput;

		/** Controls **/
		FloatOutControl mMean;
		FloatOutControl	mStd;
		FloatOutControl	mMin;
		FloatOutControl	mMax;

		/** Internal values **/
		TData _lastMean;
		TData _lastStd;
		TData _min;
		TData _max;

	public:
		StatisticalProperties(const Config & config=Config())
			:
			mInput("Input", this),
			mMean("Mean", this),
			mStd("Standard deviation (std)", this),
			mMin("Min", this),
			mMax("Max", this)
		{
			Configure( config );
			_lastMean = 0.;
			_lastStd = 0.;
			_min = 0.;
			_max = 0.;
		}

		~StatisticalProperties() {}

		bool Do()
		{
			bool result = Do( mInput.GetAudio() );

			mInput.Consume(); 

			return result;
		}
	
		bool Do(const Audio& in)
		{
			int size = in.GetSize();
			const DataArray& inb = in.GetBuffer();

			TData mean=0.;
			for (int i=0;i<size;i++)
			{
				mean += inb[i];
 				if (inb[i]>_max)
					_max = inb[i];
				else
 					if (inb[i]<_min) _min = inb[i];
			}
			_lastMean = (mean/TData(size) + _lastMean)/2.; //average with previous mean, soft change
			mMean.SendControl( _lastMean );

			mMin.SendControl( _min );
			mMax.SendControl( _max );

			TData std=0.;
			for (int i=0;i<size;i++)
			{
				std += CLAM_pow(inb[i]-_lastMean,2);
			}
 			_lastStd = (CLAM_sqrt( std/TData(size) ) + _lastStd)/2.; //average with previous std, soft change
			mStd.SendControl( _lastStd );

			return true;
		}
	
	};

};//namespace CLAM

#endif // _StatisticalProperties_
