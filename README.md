# pQsort
final project 

to enable openmp on mac you must direct clion to use gcc/g-- as the default is clang

instructions:
CLion -> Preferences -> Build, Execution, Deployment -> CMake -> CMake Options
Click the arrows in the CMake Options bar and it will open up.
Copy in these lines:

    -D CMAKE_C_COMPILER=/usr/local/bin/gcc-7
    -D CMAKE_CXX_COMPILER=/usr/local/bin/g++-7

within CMakeLists.txt, this line is already included and also works to enable openmp:

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
