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


struct Owner{
    uint32_t who=0;

    bool operator==(const Owner& other) const{
        return this->who == other.who;
    }

    Owner operator+(const int& increment) const{
        return Owner{this->who + increment};
    }
};

struct Relation{
    uint32_t with_whom=0;

    bool operator==(const Relation& other) const {
        return this->with_whom == other.with_whom;
    }
    bool operator!=(const Relation& other) const {
        return this->with_whom != other.with_whom;
    }
    bool operator<(const Relation& other) const {
        return this->with_whom < other.with_whom;
    }
    bool operator<=(const Relation& other) const {
        return this->with_whom <= other.with_whom;
    }
    bool operator>(const Relation& other) const {
        return this->with_whom > other.with_whom;
    }
    bool operator>=(const Relation& other) const {
        return this->with_whom >= other.with_whom;
    }
};


class SortedSparseMatrix {
    /// vector indicating the position where relations of an owner start
    std::vector<uint32_t> starting_positions;
    /// vector of all relations in the matrix
    std::vector<Relation> relations;

    /// updates the position of boundaries for all owners after index
    void update_boundaries(uint32_t start_index, uint32_t by_how_much);

    /// finds where relations of an owner start
    uint32_t find_position_of_relation(Owner owner, Relation relation);

    /// binary search: looks for element between low and high indexes. If not found returns where element should be
    uint32_t binary_search_in_relations(Relation relation, uint32_t low, uint32_t high);

    /// finds where relations of owner begin
    [[nodiscard]] uint32_t find_start_of_relations(Owner owner) const;

public:
    /// default constructor. Number of nodes is essential to be set to correct number!!!
    /// If too little nodes code does not resize the private vector!!!
    SortedSparseMatrix(int N_nodes, int N_relations);

    /// imports relations from file specified. Format of file should be "{owner} {relation}" separated by new line
    void import_relations_from_file(const char * filename);

    /// inserts relation and updates the boundaries vector
    void insert_relation(Owner owner, Relation relation);

    /// removes relation and updates the boundaries vector
    void remove_relation(Owner owner, Relation relation);

    /// checks if owner is in possession of relation
    [[nodiscard]] bool is_in_relation(Owner owner, Relation relation);

    /// returns number of relations belonging to owner
    [[nodiscard]] uint32_t get_number_of_relations(Owner owner) const;

    /// returns copy of relation at index
    [[nodiscard]] Relation get_relation_at_index(uint32_t index) const;

    /// returns all relations belonging to owner.
    [[nodiscard]] std::span<const Relation> get_all_relations(Owner owner) const;

    [[nodiscard]] uint32_t get_number_of_nodes() const;

    /// standard output of SortedSparseMatrix.
    /// Returns stream with information on number of nodes + lists each relation owner has
    friend std::ostream& operator<<(std::ostream& stream, const SortedSparseMatrix& obj);
};

#endif //SORTEDSPARSEMATRIX_H
