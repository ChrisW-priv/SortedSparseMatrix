add_library(SortedSparseMatrix
        src/SortedSparseMatrix.cpp
        src/SortedSparseMatrix.h)

target_include_directories(SortedSparseMatrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
