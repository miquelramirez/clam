/*
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
#ifndef RingBuffer_hxx
#define RingBuffer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInFilename.hxx> 
#include <CLAM/Audio.hxx>
#include <sndfile.hh>
#include <pthread.h>
//#include <jack/ringbuffer.h>
#include "clam_ringbuffer.h"
#include "WorkerSemaphore.hxx"

namespace CLAM
{

template <typename BaseType>
class RingBuffer
{
public:
	RingBuffer(unsigned size)
		: _overruns(0)
	{
		_rb = clam_ringbuffer_create(sizeof(BaseType)*size);
		memset(_rb->buf, 0, _rb->size);
	}
	~RingBuffer()
	{
		clam_ringbuffer_free (_rb);
	}
	bool read(BaseType * buffer, unsigned n)
	{
		unsigned nBytes = n * sizeof(BaseType);
		if (clam_ringbuffer_read_space(_rb) < nBytes) return false;
		clam_ringbuffer_read(_rb, (char*)buffer, nBytes);
		return true;
	}
	void writableRegion(BaseType *(&buffer), unsigned & len)
	{
		clam_ringbuffer_data_t writeSpace[2];
		clam_ringbuffer_get_write_vector(_rb, writeSpace);
		buffer = (BaseType*) writeSpace[0].buf;
		len = writeSpace[0].len/sizeof(BaseType);
	}
	unsigned readSpace()
	{
		return clam_ringbuffer_read_space(_rb)/sizeof(BaseType);
	}
	void advanceWrite(unsigned n)
	{
		clam_ringbuffer_write_advance(_rb, n*sizeof(BaseType));
	}
	void write(BaseType* buffer, unsigned n)
	{
		unsigned nBytes = n * sizeof(BaseType);
		unsigned actualWriten = 
			clam_ringbuffer_write(_rb, (char*) buffer, nBytes);
		if (actualWriten<nBytes) _overruns++;
	}
	bool hadOverRuns() { return _overruns>0; }
private:
	clam_ringbuffer_t * _rb;
	unsigned _overruns;
};


}// namespace CLAM
#endif // RingBuffer_hxx

