#include <algorithm>
#include <fstream>
#include "SortedSparseMatrix.h"


template<typename RelationId_type, typename Relation>
SortedSparseMatrix<RelationId_type, Relation>::SortedSparseMatrix(size_t N_nodes, size_t N_relations) {
    starting_positions.resize(N_nodes+1);
    relations.resize(N_relations);
}

template<typename RelationId_type, typename Relation>
void SortedSparseMatrix<RelationId_type, Relation>::insert_relation(RelationId_type owner, Relation relation) {
    // look for index
    auto found_element = find_position_of_relation(owner, relation);
    // if already in relation we don't want to add another relation
    if (get_relation_at_index(found_element) == relation && get_number_of_relations(owner) != 0) return;

    // if all good, insert into position
    auto position_to_insert = relations.begin() + found_element;
    relations.insert(position_to_insert, relation);
    update_boundaries(owner, 1);
}

template<typename RelationId_type, typename Relation>
void SortedSparseMatrix<RelationId_type, Relation>::remove_relation(RelationId_type owner, Relation relation) {
    auto element_found = find_position_of_relation(owner, relation);
    if (get_relation_at_index(element_found) != relation) return;

    relations.erase(relations.begin() + element_found);
    update_boundaries(owner, -1);
}

template<typename RelationId_type, typename Relation>
inline bool SortedSparseMatrix<RelationId_type, Relation>::is_in_relation(RelationId_type owner, Relation relation) const {
    auto found_element = find_position_of_relation(owner, relation);
    return get_relation_at_index(found_element) == relation && get_number_of_relations(owner) != 0;
}

template<typename RelationId_type, typename Relation>
inline RelationId_type SortedSparseMatrix<RelationId_type, Relation>::find_start_of_relations(RelationId_type owner) const  {
    return starting_positions[owner];
}

template<typename RelationId_type, typename Relation>
inline RelationId_type SortedSparseMatrix<RelationId_type, Relation>::get_number_of_relations(RelationId_type owner) const {
    return find_start_of_relations(owner+1) - find_start_of_relations(owner);
}

template<typename RelationId_type, typename Relation>
inline const Relation& SortedSparseMatrix<RelationId_type, Relation>::get_relation_at_index(RelationId_type index) const {
    return relations[index];
}

template<typename RelationId_type, typename Relation>
std::span<const Relation> SortedSparseMatrix<RelationId_type, Relation>::get_all_relations(RelationId_type owner) const {
    auto start_ = relations.begin() + find_start_of_relations(owner);
    auto end_ = relations.begin() + find_start_of_relations(owner+1);
    return {start_, end_};
}

template<typename RelationId_type, typename Relation>
inline void SortedSparseMatrix<RelationId_type, Relation>::update_boundaries(RelationId_type start_index, RelationId_type by_how_much) {
    for (auto i = start_index+1; i < starting_positions.size(); ++i) {
        starting_positions[i] += by_how_much;
    }
}

template<typename RelationId_type, typename Relation>
std::ostream &operator<<(std::ostream &stream, const SortedSparseMatrix<RelationId_type, Relation>& obj) {
    stream<<"Number of Nodes: " << obj.get_number_of_nodes() << '\n';
    RelationId_type owner = 0;
    RelationId_type index = 0;
    while (owner < obj.starting_positions.size()-1){
        std::cout << "Neighbours of node: " << owner << ":\n";
        auto N_relations = obj.get_number_of_relations(owner);
        for (int i = 0; i < N_relations; ++i) {
            std::cout << obj.get_relation_at_index(index) << '\n';
            ++index;
        }
        ++owner;
    }
    return stream;
}

template<typename RelationId_type, typename Relation>
inline RelationId_type SortedSparseMatrix<RelationId_type, Relation>::find_position_of_relation(RelationId_type owner, Relation relation) const {
     return binary_search_in_relations(relation, find_start_of_relations(owner), find_start_of_relations(owner+1));
}

template<typename RelationId_type, typename Relation>
inline RelationId_type SortedSparseMatrix<RelationId_type, Relation>::binary_search_in_relations(Relation relation, RelationId_type low, RelationId_type high) const {
    high++;
    RelationId_type mid=low;
    while (low != high) {
        mid = (low + high) / 2;

        if (relations[mid] == relation)
            return mid;
        if (relations[mid] < relation)
            low = mid+1;
        else
            high = mid-1;
    }

    return mid;
}

template<typename RelationId_type, typename Relation>
void SortedSparseMatrix<RelationId_type, Relation>::import_relations_from_file(const char *filename) {
    std::ifstream file{filename};
    if (!file) std::cout << "couldn't open file\n";

    RelationId_type owner, relation;
    while (file){
        file >> owner;
        file >> relation;
        this->insert_relation(owner, Relation{relation});
    }
}

template<typename RelationId_type, typename Relation>
inline RelationId_type SortedSparseMatrix<RelationId_type, Relation>::get_number_of_nodes() const {
    return starting_positions.size()-1;
}
