#include <algorithm>
#include <fstream>
#include "SortedSparseMatrix.h"

SortedSparseMatrix::SortedSparseMatrix(int N_nodes, int N_relations) {
    starting_positions.resize(N_nodes+1);
    relations.resize(N_relations);
}

void SortedSparseMatrix::insert_relation(Owner owner, Relation relation) {
    // if already in relation we don't want to add another relation
    // case when relation is 0 is an edge case - we init all elements to zero so all relations with 0 would be dropped
    // this is a bad fix, but it works fine and definitely will not cause problems down the line
    // alternative is to make possible scope of relations from 1 not zero (but this requires more work)
    if (is_in_relation(owner, relation) && relation.with_whom != 0) return;

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
    return get_relation_at_index(found_element) == relation;
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

std::vector<Relation> SortedSparseMatrix::get_all_relations(Owner owner) const {
    std::vector<Relation> result;
    auto n_neighbours = get_number_of_relations(owner);
    result.reserve(n_neighbours);
    auto start = find_start_of_relations(owner);
    for (auto i = start; i < start+n_neighbours; ++i) {
        result.emplace_back(result.emplace_back(relations.at(i)));
    }
    return result;
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

uint32_t SortedSparseMatrix::linear_search_in_relations(Relation relation, uint32_t low, uint32_t high) {
    uint32_t i = low;
    while (i < high) {
        auto element = relations.at(i);
        if (element >= relation) break;
        ++i;
    }
    return i;
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

