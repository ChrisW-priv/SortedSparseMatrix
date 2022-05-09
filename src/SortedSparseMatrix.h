#ifndef SORTEDSPARSEMATRIX_H
#define SORTEDSPARSEMATRIX_H

#include <vector>
#include <span>
#include <cstdint>
#include <iostream>

/* Optimised version of Sparse matrix
 * Focused on quick neighbouring nodes access
 * We split relation into owner and the rest of the relation
*/


template<typename OwnerId_type=uint16_t , typename Relation=uint32_t>
class SortedSparseMatrix {
    /// vector indicating the position where relations of an owner start
    std::vector<OwnerId_type> starting_positions;
    /// vector of all relations in the matrix
    std::vector<Relation> relations;

    /// updates the position of boundaries for all owners after index
    void update_boundaries(OwnerId_type start_index, OwnerId_type by_how_much);

    /// finds where relations of an owner start
    [[nodiscard]] OwnerId_type find_position_of_relation(OwnerId_type owner, Relation relation) const;

    /// binary search: looks for element between low and high indexes. If not found returns where element should be
    [[nodiscard]] OwnerId_type binary_search_in_relations(Relation relation, OwnerId_type low, OwnerId_type high) const;

    /// finds where relations of owner begin
    [[nodiscard]] OwnerId_type find_start_of_relations(OwnerId_type owner) const;

public:
    /// default constructor. Number of nodes is essential to be set to correct number!!!
    /// If too little nodes code does not resize the private vector!!!
    SortedSparseMatrix(size_t N_nodes, size_t N_relations);

    /// imports relations from file specified. Format of file should be "{owner} {relation}" separated by new line
    void import_relations_from_file(const char * filename);

    /// inserts relation and updates the boundaries vector
    void insert_relation(OwnerId_type owner, Relation relation);

    /// removes relation and updates the boundaries vector
    void remove_relation(OwnerId_type owner, Relation relation);

    /// checks if owner is in possession of relation
    [[nodiscard]] bool is_in_relation(OwnerId_type owner, Relation relation) const;

    /// returns number of relations belonging to owner
    [[nodiscard]] OwnerId_type get_number_of_relations(OwnerId_type owner) const;

    /// returns copy of relation at index
    [[nodiscard]] const Relation& get_relation_at_index(OwnerId_type index) const;

    /// returns all relations belonging to owner.
    [[nodiscard]] std::span<const Relation> get_all_relations(index_size_type owner) const;

    [[nodiscard]] OwnerId_type get_number_of_nodes() const;

    /// standard output of SortedSparseMatrix.
    /// Returns stream with information on number of nodes + lists each relation owner has
    friend std::ostream& operator<<(std::ostream& stream, const SortedSparseMatrix& obj);
};

#endif //SORTEDSPARSEMATRIX_H
