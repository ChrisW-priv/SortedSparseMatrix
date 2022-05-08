set(CMAKE_CXX_STANDARD 20)

add_library(SortedSparseMatrix
        src/SortedSparseMatrix.cpp
        src/SortedSparseMatrix.h)

target_include_directories(SortedSparseMatrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
