// #ifndef SEARCHABLE_TREE_BAG_HPP
// #define SEARCHABLE_TREE_BAG_HPP

// #include "searchable_bag.hpp"
// #include "tree_bag.hpp"

// class searchable_tree_bag : public tree_bag, public searchable_bag {
// public:
// 	searchable_tree_bag(); 
// 	searchable_tree_bag(const searchable_tree_bag &other); 
// 	searchable_tree_bag &operator=(const searchable_tree_bag &other); 
// 	~searchable_tree_bag(); 
// 	bool has(int val) const;

// private:
// 	bool search_tree(node *n, int val) const;
// };

// #endif















#ifndef SEARCHABLE_tree_BAG_HPP
#define SEARCHABLE_tree_BAG_HPP


#include "tree_bag.hpp"
#include "searchable_bag.hpp"

class searchable_tree_bag : public tree_bag, public searchable_bag {
public:
	searchable_tree_bag(); 
	searchable_tree_bag(const searchable_tree_bag &other); 
	searchable_tree_bag &operator=(const searchable_tree_bag &other); 
	~searchable_tree_bag(); 

	bool has(int val) const;

private:
	bool search_tree(node *n, int val) const;
};

#endif
