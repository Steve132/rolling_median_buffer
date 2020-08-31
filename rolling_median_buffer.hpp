#ifndef ROLLING_MEDIAN_BUFFER_HPP
#define ROLLING_MEDIAN_BUFFER_HPP

#include<memory>
#include<vector>
#include<array>
#include<algorithm>

#include "simd_traits.hpp"


template<class T,class simd_reg,unsigned int N>
struct simd_med
{
	static simd_reg median(std::array<simd_reg,N>& a);
};

template<class T,unsigned N>
class rolling_median_buffer
{
protected:
	using simd_reg=typename simd_traits<T>::register_type;
	static constexpr unsigned int simd_align=simd_traits<T>::alignment;
	static constexpr unsigned int simd_lane_count=simd_traits<T>::lane_count;
	
	std::vector<simd_reg,typename simd_traits<T>::allocator> entries;
	
	size_t head;
public:
	using vector_type=std::vector<T,simd_allocator<T,simd_align>>;
	
	//N*((n/simd_lane_count)+(n % simd_lane_count ? 1 : 0))
	rolling_median_buffer(size_t n):entries((n*N)/simd_lane_count+((n % simd_lane_count) ? N : 0)),head(0)
	{}
	void poppush(const T* bufin,T* bufout,size_t offset,size_t count)
	{
		/*if(((reinterpret_cast<std::intptr_t>(bufin)%simd_align)!=0) 
		||((reinterpret_cast<std::intptr_t>(bufout)%simd_align)!=0))
		{
			throw std::runtime_error("IO buffers not aligned");
		}*/
		simd_reg* entries_ptr=entries.data();
		const simd_reg* bufin_simd=reinterpret_cast<const simd_reg*>(bufin);
		simd_reg* bufout_simd=reinterpret_cast<simd_reg*>(bufout);
		
		size_t boffset=offset/simd_lane_count;
		size_t bcount=count/simd_lane_count;
		
		#pragma omp for
		for(size_t i=boffset;i<bcount;i++)
		{
			simd_reg* buckets_start=entries_ptr+i*N;
			buckets_start[head]=simd_traits<T>::load_u(bufin_simd+i);
 			std::array<simd_reg,N> cpy=*reinterpret_cast<const std::array<simd_reg,N>*>(buckets_start);
			simd_traits<T>::store_u(bufout_simd+i,simd_med<T,simd_reg,N>::median(cpy));
		}
	}
	void push_complete()
	{
		head=(head+1) % N;
	}
	//size_t size() const { return npoints; }
};

//https://en.wikipedia.org/wiki/Batcher_odd%E2%80%93even_mergesort
/*
template<class T>
inline typename rolling_median_buffer<T,2>::simd_reg rolling_median_buffer<T,2>::median_blocks(const typename rolling_median_buffer<T,2>::simd_reg* b)
{
	cmpswap_blocks(b[0],b[1]);
	return b[1];
}*/


#include "rolling_median_buffer.inl"


#endif

