#ifndef SIMD_TRAITS_HPP
#define SIMD_TRAITS_HPP

#include<memory>

template<class T,unsigned int A>
struct simd_allocator {
	typedef T value_type;
	simd_allocator() noexcept {}
	template <class U> simd_allocator (const simd_allocator<U,A>&) noexcept {}
	T* allocate (std::size_t n);
	void deallocate (T* p, std::size_t n);
	template<class U>
	constexpr bool operator== (const simd_allocator<U,A>&) noexcept
	{return true;}
	template <class U>
	constexpr bool operator!= (const simd_allocator<U,A>&) noexcept
	{return false;}
	template< class U > struct rebind { typedef simd_allocator<U,A> other; }; 
};


template<class T,unsigned int LANE_COUNT=0>
struct simd_traits
{
	static constexpr unsigned int lane_count=LANE_COUNT==0 ? 1 : LANE_COUNT;
	using register_type=std::array<T,lane_count>;
	static constexpr unsigned int alignment=alignof(register_type);
	using allocator=simd_allocator<register_type,alignment>;
};


#ifdef __SSE__
#include <xmmintrin.h>

template<class T,unsigned int A>
inline T* simd_allocator<T,A>::allocate (std::size_t n) { return static_cast<T*>(_mm_malloc(n*sizeof(T),A)); }
template<class T,unsigned int A>
void simd_allocator<T,A>::deallocate (T* p, std::size_t n) { _mm_free(p); }

#else
	
template<class T,unsigned int A>
inline T* simd_allocator<T,A>::allocate (std::size_t n) { return static_cast<T*>(::operator new(n*sizeof(T))); }
template<class T,unsigned int A>
void simd_allocator<T,A>::deallocate (T* p, std::size_t n) { ::delete(p); }

#endif


#ifdef __AVX2__
#include <immintrin.h>

template<>
struct simd_traits<uint8_t,0>
{
	static constexpr unsigned int lane_count=32;
	using register_type=__m256i;
	static constexpr unsigned int alignment=alignof(register_type);
	using allocator=simd_allocator<register_type,alignment>;
};

#endif

#ifdef __ARM_NEON
template<>
struct simd_traits<uint8_t,0>
{
	static constexpr unsigned int lane_count=16;
	using register_type=int8x16_t;
	static constexpr unsigned int alignment=alignof(register_type);
	using allocator=simd_allocator<register_type,alignment>;
};
#endif

#endif
