#ifndef AlignedAllocator_hxx
#define AlignedAllocator_hxx

#include <fftw3.h>
#include <limits>

template<typename T>
class AlignedAllocator //: std::allocator<T>
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
/*
	inline explicit AlignedAllocator() {}
	inline ~AlignedAllocator() {}
	inline explicit AlignedAllocator(AlignedAllocator const&) {}
	template<typename U>
	inline explicit AlignedAllocator(AlignedAllocator<U> const&) {}
	inline pointer address(reference r) { return &r; }
	inline const_pointer address(const_reference r) { return &r; }
*/
	// memory allocation

	inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
	{
		size_t bytes = cnt * sizeof(T);
		return reinterpret_cast<pointer>(
			fftw_malloc(bytes)); // The standard one
	}
	inline void deallocate(pointer p, size_type)
	{ 
		fftw_free(p); 
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
