#ifndef FIXED_FIFO_HPP
#define FIXED_FIFO_HPP

template<class T,uint8_t N>
struct fixed_fifo
{
protected:	
	std::array<uint8_t,N> data;
	ptrdiff_t head;
public:
	fixed_fifo():head(0) {}
	T push(const T& t)
	{
		T out=data[head];
		data[head++]=t;
		head %= N;
		return out;
	}
	const std::array<uint8_t,N>& buffer() const { return data; }
};

#endif
