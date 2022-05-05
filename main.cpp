#include <iostream>
#include "SortedSparseMatrix.h"

int main() {
    SortedSparseMatrix matrix{10, 30};
    matrix.import_relations_from_file("../generic_relations.txt");

    std::cout << matrix;
}
