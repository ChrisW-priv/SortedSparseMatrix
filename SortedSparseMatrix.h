#ifndef SORTEDSPARSEMATRIX_H
#define SORTEDSPARSEMATRIX_H

#include <vector>
#include <span>
#include <cstdint>
#include <iostream>

/* Optimised version of Sparse matrix
 * Focused on quick neighbouring nodes access
 * We split relation into owner and the rest of the relation
 * Currently using linear search. Soon to implement working
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
    std::vector<uint32_t> starting_positions;
    std::vector<Relation> relations;

    void update_boundaries(uint32_t start_index, uint32_t by_how_much);
    uint32_t find_position_of_relation(Owner owner, Relation relation);
    uint32_t binary_search_in_relations(Relation relation, uint32_t low, uint32_t high);
public:
    SortedSparseMatrix()=default;
    SortedSparseMatrix(int N_nodes, int N_relations);

    void import_relations_from_file(const char * filename);

    void insert_relation(Owner owner, Relation relation);
    void remove_relation(Owner owner, Relation relation);
    [[nodiscard]] bool is_in_relation(Owner owner, Relation relation);
    [[nodiscard]] uint32_t find_start_of_relations(Owner owner) const;
    [[nodiscard]] uint32_t get_number_of_relations(Owner owner) const ;
    [[nodiscard]] Relation get_relation_at_index(uint32_t index) const;
    [[nodiscard]] std::span<const Relation> get_all_relations(Owner owner) const;

    friend std::ostream& operator<<(std::ostream& stream, const SortedSparseMatrix& obj);

};

#endif //SORTEDSPARSEMATRIX_H
