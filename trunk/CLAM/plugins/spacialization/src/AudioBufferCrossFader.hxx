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

#ifndef AudioBufferCrossFader_hxx
#define AudioBufferCrossFader_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>

namespace CLAM
{

class AudioBufferCrossFader : public Processing
{
	InPort<Audio> _in1;
	InPort<Audio> _in2;
	OutPort<Audio> _out;
public:
	const char* GetClassName() const { return "AudioBufferCrossFader"; }
	AudioBufferCrossFader(const Config& config = Config()) 
		: _in1("FadingIn", this)
		, _in2("FadingOut", this)
		, _out("CrossFadedBuffer", this) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		const Audio& in1 = _in1.GetData();
		const Audio& in2 = _in2.GetData();
		CLAM_ASSERT(in1.GetSize()==in2.GetSize(),
			"AudioBufferCrossFader: Input audio buffers should be the same size");
		const unsigned size = in1.GetSize();
		Audio& out = _out.GetData();
		const TData* inpointer1 = in1.GetBuffer().GetPtr();
		const TData* inpointer2 = in2.GetBuffer().GetPtr();
		if (out.GetSize()!=size)
		{
			std::cout << "AudioBufferCrossFader: Doing a audio resize. GetSize() " << out.GetSize() <<  " "<< &_out.GetData()  << std::endl;
			out.SetSize(size);
			std::cout << "AudioBufferCrossFader: after resize " << out.GetSize() << std::endl;
		}
		TData* outpointer = out.GetBuffer().GetPtr();
		TData step = 1./size;
		TData level = 0;
		for (unsigned i = 0; i<size; i++, level+=step)
		{
			outpointer[i]=level*inpointer1[i];
			outpointer[i]+=(1-level)*inpointer2[i];
		}

		// Tell the ports this is done
		_in1.Consume();
		_in2.Consume();
		_out.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // AudioBufferCrossFader_hxx
