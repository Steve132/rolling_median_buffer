cmake_minimum_required(VERSION 3.10)


add_library(rolling_median_buffer
	rolling_median_buffer.hpp
	rolling_median_buffer.cpp
	rolling_median_buffer.inl
)
set_target_properties(rolling_median_buffer PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

add_executable(rmb_test
	main.cpp
)

set_target_properties(rmb_test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(rmb_test PUBLIC OpenMP::OpenMP_CXX)
endif()
