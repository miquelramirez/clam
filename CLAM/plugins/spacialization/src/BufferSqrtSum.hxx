/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef BUFFERSQRTSUM_INCLUDED
#define BUFFERSQRTSUM_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/OutControl.hxx>

#include <functional>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>

namespace CLAM
{

/**
*/
class BufferSqrtSum : public Processing
{
	InPort<Audio> _in1;
	FloatOutControl _outputControl;

public:
	const char* GetClassName() const { return "BufferSqrtSum"; }
	BufferSqrtSum(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _outputControl("Value", this)
	{
		Configure( config );
	}
 
	bool Do()
	{
		const Audio& in1 = _in1.GetData();
		const unsigned size = in1.GetSize();
		/*const*/ TData* first = in1.GetBuffer().GetPtr();
		/*const*/ TData* last = first + size;

		// in place transform
		std::transform(first, last, first, first, std::multiplies<TData>());
		
		TData sum = std::accumulate(first, last, static_cast<TData>(0));
		TData result = std::sqrt(sum);
		
		//std::cout << "buffer=" << std::endl;
		//std::copy(first, last, std::ostream_iterator<TData>(std::cout, " "));
		//std::cout << std::endl;

		_outputControl.SendControl(static_cast<TControlData>(result));

		// Tell the ports this is done
		_in1.Consume();
		return true;
	}
};

} // namespace CLAM

#endif 

