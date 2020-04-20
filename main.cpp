#include "rolling_median_buffer.hpp"
#include<iostream>
#include<omp.h>

int main()
{	
	//size_t nelems=128;
	size_t nelems=1024*1024*1024;
	rolling_median_buffer<uint8_t,8>::vector_type buf(nelems);
	rolling_median_buffer<uint8_t,8> rmb(nelems);
	
	
	
	#pragma omp parallel
	for(unsigned int i=0;i<10;i++)
	{
		//std::fill(std::begin(buf),std::end(buf),i+1);
		//std::cout << i+1 << std::endl;
		#pragma omp for
		for(unsigned int z=0;z<omp_get_num_threads();z++)
		{
			size_t blockcount=nelems/omp_get_num_threads();
			rmb.poppush(&buf[0],&buf[0],blockcount*z,blockcount);
		}
		//implicit barrier
        #pragma omp single
        {
			rmb.push_complete();
		}
		//for(size_t c=0;c<nelems;c++) std::cout << (int)buf[c] << ",";
		//std::cout << std::endl << (int)buf[0] << std::endl;
	}
	
	
	
	
	return 0;
}

//rolling_median_buffer<uint8_t,4>::vector_type buf(128);
//rolling_median_buffer<uint8_t,4>::vector_type buf(128);
//std::vector<uint32_t> buf(128);
//std::cout << (void*)buf.data() << std::endl;
//rolling_median_buffer<uint8_t,4> rmb(128);
