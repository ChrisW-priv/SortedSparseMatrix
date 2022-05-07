#include <algorithm>
#include <fstream>
#include "SortedSparseMatrix.h"

SortedSparseMatrix::SortedSparseMatrix(int N_nodes, int N_relations) {
    starting_positions.resize(N_nodes+1);
    relations.resize(N_relations);
}

void SortedSparseMatrix::insert_relation(Owner owner, Relation relation) {
    // if already in relation we don't want to add another relation
    if (is_in_relation(owner, relation)) return;

    auto position_to_insert = relations.begin() + find_position_of_relation(owner, relation);
    relations.insert(position_to_insert, relation);
    update_boundaries(owner.who, 1);
}

void SortedSparseMatrix::remove_relation(Owner owner, Relation relation) {
    auto element_found = find_position_of_relation(owner, relation);
    if (get_relation_at_index(element_found) != relation) return;

    relations.erase(relations.begin() + element_found);
    update_boundaries(owner.who, -1);
}

bool SortedSparseMatrix::is_in_relation(Owner owner, Relation relation) {
    auto found_element = find_position_of_relation(owner, relation);
    return get_relation_at_index(found_element) == relation && get_number_of_relations(owner) != 0;
}

uint32_t SortedSparseMatrix::find_start_of_relations(Owner owner) const  {
    return starting_positions.at(owner.who);
}

uint32_t SortedSparseMatrix::get_number_of_relations(Owner owner) const {
    return find_start_of_relations(owner+1) - find_start_of_relations(owner);
}

Relation SortedSparseMatrix::get_relation_at_index(uint32_t index) const {
    return relations.at(index);
}

std::span<const Relation> SortedSparseMatrix::get_all_relations(Owner owner) const {
    auto start_ = relations.begin() + find_start_of_relations(owner);
    auto end_ = relations.begin() + find_start_of_relations(owner+1);
    return {start_, end_};
}

void SortedSparseMatrix::update_boundaries(uint32_t start_index, uint32_t by_how_much) {
    for (auto i = start_index+1; i < starting_positions.size(); ++i) {
        starting_positions[i] += by_how_much;
    }
}

std::ostream &operator<<(std::ostream &stream, const SortedSparseMatrix& obj) {
    stream<<"Number of Nodes: " << obj.starting_positions.size()-1 << '\n';
    uint32_t owner = 0;
    uint32_t index = 0;
    while (owner < obj.starting_positions.size()-1){
        std::cout << "Neighbours of node: " << owner << ":\n";
        auto N_relations = obj.get_number_of_relations(Owner{owner});
        for (int i = 0; i < N_relations; ++i) {
            std::cout << obj.get_relation_at_index(index).with_whom << '\n';
            ++index;
        }
        ++owner;
    }
    return stream;
}

uint32_t SortedSparseMatrix::find_position_of_relation(Owner owner, Relation relation) {
     return binary_search_in_relations(relation, find_start_of_relations(owner), find_start_of_relations(owner+1));
}

uint32_t SortedSparseMatrix::binary_search_in_relations(Relation relation, uint32_t low, uint32_t high) {
    high++;
    uint32_t mid=low;
    while (low != high) {
        mid = low + (high - low) / 2;

        if (relations.at(mid) == relation)
            return mid;
        if (relations.at(mid) < relation)
            low = mid+1;
        else
            high = mid-1;
    }

    return mid;
}

void SortedSparseMatrix::import_relations_from_file(const char *filename) {
    std::ifstream file{filename};
    if (!file) std::cout << "couldn't open file\n";

    uint32_t owner, relation;
    while (file){
        file >> owner;
        file >> relation;
        this->insert_relation(Owner{owner}, Relation{relation});
    }
}

