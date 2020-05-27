
namespace
{
template<class T,class simd_reg>
void cswap(simd_reg& a,simd_reg& b);

#if defined(__AVX2__)
#include <immintrin.h>

template<>
inline void cswap<uint8_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epu8(a,b);a=_mm256_min_epu8(a,tmp);
}
template<>
inline void cswap<int8_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epi8(a,b);a=_mm256_min_epi8(a,tmp);
}
template<>
inline void cswap<uint16_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epu16(a,b);a=_mm256_min_epu16(a,tmp);
}
template<>
inline void cswap<int16_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epi16(a,b);a=_mm256_min_epi16(a,tmp);
}
template<>
inline void cswap<uint32_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epu32(a,b);a=_mm256_min_epu32(a,tmp);
}
template<>
inline void cswap<int32_t,__m256i>(__m256i& a,__m256i& b) {
	__m256i tmp=b;b=_mm256_max_epi32(a,b);a=_mm256_min_epi32(a,tmp);
}


#elif defined(__ARM_NEON)
template<>
inline void cswap<uint8_t,int8x16_t>(int8x16_t& a,int8x16_t& b) {
	int8x16_t tmp=b;b=vmaxq_u8(a,b);a=vminq_u8(a,tmp);
}
#else
template<>
inline void cswap<uint8_t,std::array<uint8_t,1> >(std::array<uint8_t,1>& a,std::array<uint8_t,1>& b) {
	uint8_t* aptr=reinterpret_cast<uint8_t*>(&a);
	uint8_t* bptr=reinterpret_cast<uint8_t*>(&b);
	for(unsigned int i=0;i<sizeof(std::array<uint8_t,1>);i++)
	{
		if(*aptr < *bptr) { std::swap(*aptr,*bptr); }
	}
}
#endif



}

template<class T,class simd_reg,unsigned int N,unsigned B,unsigned E>
struct simd_sort
{
/*	static void sort(std::array<simd_reg,N>& a)
	{
		simd_sort<simd_reg,N,B,(B+E)/2>::sort(a);
		simd_sort<simd_reg,N,(B+E)/2,E>::sort(a);
	}*/
	static void sort(std::array<simd_reg,N>& a);
};
template<class T,class simd_reg,unsigned int N,unsigned B>
struct simd_sort<T,simd_reg,N,B,B>
{
	static void sort(std::array<simd_reg,N>& a)
	{}
};
template<class T,class simd_reg,unsigned int N>
struct simd_sort<T,simd_reg,N,0,2>
{
	static void sort(std::array<simd_reg,N>& a)
	{
		cswap<T>(a[0],a[1]);
	}
};
template<class T,class simd_reg,unsigned int N>
struct simd_sort<T,simd_reg,N,0,4>
{
	static void sort(std::array<simd_reg,N>& a)
	{
		cswap<T>(a[0],a[1]);
		cswap<T>(a[2],a[3]);
		cswap<T>(a[0],a[2]);
		cswap<T>(a[1],a[3]);
		cswap<T>(a[1],a[2]);
	}
};
template<class T,class simd_reg,unsigned int N>
struct simd_sort<T,simd_reg,N,0,8>
{
	static void sort(std::array<simd_reg,N>& a)
	{
		cswap<T>(a[0],a[1]);
		cswap<T>(a[2],a[3]);
		cswap<T>(a[0],a[2]);
		cswap<T>(a[1],a[3]);
		cswap<T>(a[1],a[2]);
		
		cswap<T>(a[4],a[5]);
		cswap<T>(a[6],a[7]);
		cswap<T>(a[4],a[6]);
		cswap<T>(a[5],a[7]);
		cswap<T>(a[5],a[6]);

		cswap<T>(a[0],a[4]);
		cswap<T>(a[1],a[5]);
		cswap<T>(a[2],a[6]);
		cswap<T>(a[3],a[7]);

		cswap<T>(a[2],a[4]);
		cswap<T>(a[3],a[5]);

		//cswap<T>(a[1],a[2]);
		cswap<T>(a[3],a[4]);
		//cswap<T>(a[5],a[6]);
	}
};
template<class T,class simd_reg,unsigned int N>
inline simd_reg simd_med<T,simd_reg,N>::median(std::array<simd_reg,N>& a)
{
	simd_sort<T,simd_reg,N,0,N>::sort(a);
	return a[N/2];
}

