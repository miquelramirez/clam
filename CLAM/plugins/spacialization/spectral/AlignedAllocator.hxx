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

#ifndef AlignedAllocator_hxx
#define AlignedAllocator_hxx

#ifdef AVOID_FFTW 
#include <cstdlib>
#else
#include <fftw3.h>
#endif
#include <limits>

template<typename T>
class AlignedAllocator
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template<typename U>
	struct rebind
	{
		typedef AlignedAllocator<U> other;
	};

	// memory allocation
	inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
	{
		size_t bytes = cnt * sizeof(T);
#ifdef AVOID_FFTW
		void * result = 0;
		// NOTE: on OSX, posix_memalign requires SDK 10.6
		if (posix_memalign(&result, 16, bytes)) return 0;
		return reinterpret_cast<pointer>(result);
#else
		return reinterpret_cast<pointer>(
			fftwf_malloc(bytes)); // The standard one
#endif
	}
	inline void deallocate(pointer p, size_type)
	{
#ifdef AVOID_FFTW 
		std::free(p);
#else
		fftwf_free(p);
#endif
	}

	inline size_type max_size() const
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	inline void construct(pointer p, const T& t) { new(p) T(t); }
	inline void destroy(pointer p) { p->~T(); }

	inline bool operator==(AlignedAllocator const&) { return true; }
	inline bool operator!=(AlignedAllocator const& a) { return !operator==(a); }
};
#endif//AlignedAllocator_hxx
