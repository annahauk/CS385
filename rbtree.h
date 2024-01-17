/*******************************************************************************
 * Name        : rbtree.h
 * Author      : Anna Hauk, Christopher Kalish
 * Version     : 1.0
 * Date        : 11-17-2023
 * Description : Implementation of red-black tree.
 * Pledge      : I pledge my Honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#ifndef RBTREE_H_
#define RBTREE_H_

#include "node.h"
#include "tree.h"
#include "treeprinter.h"
#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

using namespace std;

// Forward declaration
template <typename K, typename V>
class RedBlackTree;

/**
 * tree_exception class
 * Demonstrates how you can write your own custom exceptions in C++.
 */
class tree_exception: public std::exception {
public:
    tree_exception(const std::string &message) : message_{message} { }

    ~tree_exception() throw() { }

    virtual const char* what() const throw() {
        return message_.c_str();
    }

private:
    std::string message_;
};

template <typename K, typename V>
class RedBlackTreeIterator {
public:
    /**
     * Constructor
     */
    RedBlackTreeIterator() : node_ptr{nullptr}, tree{nullptr} { }

    /**
     * Equality operator. Compares node pointers.
     */
    bool operator==(const RedBlackTreeIterator &rhs) const {
        return node_ptr == rhs.node_ptr;
    }

    /**
     * Inequality operator. Compares node pointers.
     */
    bool operator!=(const RedBlackTreeIterator &rhs) const {
        return node_ptr != rhs.node_ptr;
    }

    /**
     * Dereference operator. Returns a reference to the Node pointed to
     * by node_ptr.
     * REFERENCE TO NODE POINTED TO BY NODE_PTR (actual value in the node)
     */
    Node<K, V>& operator*() const {
        return *node_ptr;
    }

    /**
     * Dereference operator. Returns a pointer to the Node pointed to
     * by node_ptr.
     * POINTER TO NODE POINTED TO BY NODE_PTR
     */
    Node<K, V>* operator->() const {
        return node_ptr;
    }

    /**
     * Preincrement operator. Moves forward to next larger value.
     * finds the next value bigger than the smallest
     */
    RedBlackTreeIterator& operator++() {
        Node<K, V> *p;

        if (node_ptr == nullptr) {
            // ++ from end(). Get the root of the tree.
            node_ptr = tree->root_;

            // Error, ++ requested for an empty tree.
            if (node_ptr == nullptr)
                throw tree_exception(
                        "RedBlackTreeIterator operator++(): tree empty");

            // Move to the smallest value in the tree, which is the first node
            // in an inorder traversal.
            while (node_ptr->left != nullptr) { //if left subtree isn't empty go point to it
                node_ptr = node_ptr->left; //iterates all the way down to left most left subtree
            }
        } else { //else, node_ptr != nullptr
            if (node_ptr->right != nullptr) { //checks the right tree
                // Successor is the leftmost node of right subtree.
                node_ptr = node_ptr->right;

                while (node_ptr->left != nullptr) {
                    node_ptr = node_ptr->left;
                }
            } else {
                // Have already processed the left subtree, and
                // there is no right subtree. Move up the tree,
                // looking for a parent for which node_ptr is a left child,
                // stopping if the parent becomes nullptr (or in this case,
                // root_parent_. A non-null parent is the successor. If parent
                // is nullptr, the original node was the last node inorder, and
                // its successor is the end of the list.
                p = node_ptr->parent;
                while (p != nullptr && node_ptr == p->right) {
                    node_ptr = p;
                    p = p->parent;
                }

                // If we were previously at the rightmost node in
                // the tree, node_ptr = nullptr, and the iterator specifies
                // the end of the list.
                node_ptr = p;
            }
        }

        return *this;
    }

    /**
     * Postincrement operator. Moves forward to next larger value.
     */
    RedBlackTreeIterator operator++(int) {
        RedBlackTreeIterator tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    }

private:
    // node_ptr is the current location in the tree. We can move
    // freely about the tree using left, right, and parent.
    // tree is the address of the RedBlackTree object associated
    // with this iterator. It is used only to access the
    // root pointer, which is needed for ++ and --
    // when the iterator value is end().
    Node<K, V> *node_ptr;
    RedBlackTree<K, V> *tree;
    friend class RedBlackTree<K, V>;

    /**
     * Constructor used to construct an iterator return value from a tree
     * pointer.
     */
    RedBlackTreeIterator(Node<K, V> *p, RedBlackTree<K, V> *t) :
        node_ptr(p), tree(t) { }
};

template<typename K, typename V>
class RedBlackTree : public Tree {
public:
    typedef RedBlackTreeIterator<K, V> iterator;

    /**
     * Constructor to create an empty red-black tree.
     */
    explicit RedBlackTree() : root_{nullptr}, size_{0} { }

    /**
     * Constructor to create a red-black tree with the elements from the
     * vector.
     */
    explicit RedBlackTree(std::vector< std::pair<K, V> > &elements) :
                                            root_(nullptr), size_(0) {
        insert_elements(elements);
    }

    /**
     * Destructor.
     */
    ~RedBlackTree() {
        delete_tree(root_);
    }

    /**
     * Inserts elements from the vector into the red-black tree.
     * Duplicate elements are not inserted.
     */
    void insert_elements(std::vector< std::pair<K, V> > &elements) {
        for (size_t i = 0, len = elements.size(); i < len; ++i) {
            try {
                insert(elements[i].first, elements[i].second);
            } catch (const tree_exception &te) {
                std::cerr << "Warning: " << te.what() << std::endl;
            }
        }
    }

    /**
     * Inserts a key-value pair into the red black tree.
     * const iterator &it indicates where to start the search for the place to
     * insert the node. If it == end(), the search starts at the root.
     * Must throw a tree_exception if attempting to insert a key that is
     * already present in the tree.
     */
    void insert(const iterator &it, const std::pair<K, V> &key_value) {
        //Given: 217 - 225
        // Set x to the root if it is the end and set x to it.pointer if otherwise
        ostringstream oss;
        Node<K, V> *x, *y;
        if (it != end()) {
            x = it.node_ptr;
            y = x->parent;
        } else {
            x = root_;
            y = nullptr;
        }
        // Create new node
        Node<K, V> *z = new Node<K, V>(key_value.first, key_value.second);
        //Until we reach a leaf
        while(x != nullptr){
            // Move up y
            y = x;
            // If z is already in the tree
            if(z->key() == x->key()){
                // Create an exception and delete the new node
                oss << "Attempt to insert duplicate key '" << key_value.first << "'.";
                delete z;
                throw tree_exception(oss.str());
            }else if(z->key() < x->key()){
                x = x->left;
            }else{
                x = x->right;
            }
            // Move up x
        }
        // Update size and pointers fo
        size_++;
        z->parent = y;
        z->left = nullptr;
        z->right = nullptr;
        if(y == nullptr){ //if tree empty
            root_ = z;
            z->color = BLACK;
            return;
        }else if(z->key() < y->key()){
            y->left = z;
        }else{
            y->right = z;
        }
        z->color = RED;
        insert_fixup(z); //restores the red-black properties
        
        /*
        RB-INSERT(T, z)
        y = null
        x = T.root
        while x != null
            y = x
            if z.key < x.key
                x = x.left
            else x = x.right
        z.p = y
        if y == null
            T.root = z
        else if z.key < y.key
            y.left = z
        else y.right = z
        z.left = null
        z.right = null
        z.color = RED
        RB-INSERT-FIXUP(z) takes care of cases 1-3
        */
    }

    /**
     * Inserts a key-value pair into the red-black tree.
     */
    void insert(const K &key, const V &value) {
        iterator e = end();
        insert(e, std::pair<K, V>(key, value));
    }

    /**
     * Returns an ASCII representation of the red-black tree.
     */
    std::string to_ascii_drawing() {
        BinaryTreePrinter<K, V> printer(root_);
        return printer.to_string();
    }

    /**
     * Returns the height of the red-black tree.
     */
    int height() const {
        return height(root_);
    }

    /**
     * Returns the number of nodes in the red-black tree.
     */
    size_t size() const {
        return size_;
    }

    /**
     * Returns the leaf count of the red-black tree.
     */
    size_t leaf_count() const {
        return leaf_count(root_);
    }

    /**
     * Returns the internal node count of the red-black tree.
     */
    size_t internal_node_count() const {
        return internal_node_count(root_);
    }

    /**
     * Returns the diameter of the red-black tree rooted at node.
     * Diameter is defined as the length of the longest path between
     * two (non-null) leaves in the tree. The path does not necessarily have to
     * pass through the root.
     */
    size_t diameter() const {
        return diameter(root_);
    }

    /**
     * Returns the max width of the red-black tree, i.e. the largest number of
     * nodes on any level.
     */
    size_t max_width() const {
        size_t max_width = 0;
        for (int i = 0, h = height(root_) + 1; i < h; ++i) {
            size_t w = width(root_, i);
            if (w > max_width) {
                max_width = w;
            }
        }
        return max_width;
    }

    /**
     * Returns the successful search cost, i.e. the average number of nodes
     * visited to find a key that is present.
     */
    double successful_search_cost() const {
        return size_ == 0 ? 0 : 1 + (double)sum_levels() / size_;
    }

    /**
     * Returns the unsuccessful search cost, i.e. the average number of nodes
     * visited to find a key that is not present.
     */
    double unsuccessful_search_cost() const {
        return (double)sum_null_levels() / null_count();
    }

    /**
     * Searches for item. If found, returns an iterator pointing
     * at it in the tree; otherwise, returns end().
     */
    iterator find(const K &key) {
        Node<K, V> *x = root_;
        while (x != nullptr) {
            const K& current_key = x->key();
            if (key == current_key) {
                break; // Found!
            } else if (key < current_key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(x, this);
    }

    /**
     * Returns an iterator pointing to the first item in order.
     */
    iterator begin() {
        Node<K, V> *curr = root_;

        // if the tree is not empty, the first node
        // in order is the farthest node left from root
        if (curr != nullptr) {
            while (curr->left != nullptr) {
                curr = curr->left;
            }
        }

        // build return value using private constructor
        return iterator(curr, this);
    }

    /**
     * Returns an iterator pointing just past the end of the tree data.
     */
    iterator end() {
        return iterator(nullptr, this);
    }

private:
    Node<K, V> *root_;
    size_t size_;
    friend class RedBlackTreeIterator<K, V>;

    /**
     * Deletes all nodes from the red-black tree.
     */
    void delete_tree(Node<K, V> *n) {//DONE
        if(n == nullptr){
            return;
        }
        delete_tree(n->left);
        delete_tree(n->right);
        delete n;
        return;
    }

    /**
     * Fixup method described on p. 316 of CLRS.
     */
    void insert_fixup(Node<K, V> *z) { //DONE
        Node<K, V> *y = nullptr;
        while(z != nullptr && z != root_ && z->parent->color == RED){
            //z's parent is a left child
            if(z->parent == z->parent->parent->left){
                y = z->parent->parent->right;
                if(y != nullptr && y->color == RED){//z's uncle y is red
                    z->parent->color = BLACK;       //Case 1
                    y->color = BLACK;               //Case 1
                    z->parent->parent->color = RED; //Case 1
                    z = z->parent->parent;
                }else{
                    if(z == z->parent->right){ //z's uncle is black and z is right child
                        z = z->parent;                  //Case 2a
                        left_rotate(z);                 //Case 2a
                    }
                    z->parent->color = BLACK;           //Case 3a
                    z->parent->parent->color = RED;     //Case 3a
                    right_rotate(z->parent->parent);     //Case 3a
                }
            }else{
                y = z->parent->parent->left;
                if(y != nullptr && y->color == RED){ //z's uncle y is red
                    z->parent->color = BLACK;       //Case 1
                    y->color = BLACK;               //Case 1
                    z->parent->parent->color = RED; //Case 1
                    z = z->parent->parent;
                //z’s uncle y is black and z is a left child
                }else{
                    if(z == z->parent->left){ //z's uncle is black and z is right child
                        z = z->parent;                  //Case 2b
                        right_rotate(z);                 //Case 2b
                    }
                    z->parent->color = BLACK;           //Case 3b
                    z->parent->parent->color = RED;     //Case 3b
                    left_rotate(z->parent->parent);     //Case 3b
                }
            }
        }
        // Last line below
        root_->color = BLACK;
    }

    /**
     * Left-rotate method described on p. 313 of CLRS.
     */
    void left_rotate(Node<K, V> *x) { //DONE
        /*
        x is originally "root"
        y is right child of x
        left child stays; left child of y becomes right
            x                y
          /   \            /   \
        a      y    >>    x     z
             /   \      /   \
            b     z    a     b
        */
        //K == Key, V == Value
        //Node<K,V>* is a pointer to a node since we're modify the shape of
        //the tree and not the contents. only pointers are changed

        Node<K,V>* y = x->right; //Save the right child of node x in a variable y
        x->right = y->left; //Update the right child of node x to be the left child of y
        if(y->left != nullptr){//If there is a left child of y
            //update its parent pointer to point to x
            y->left->parent = x; //updates the b to attach to x as right subchild
        }
        //Update the parent pointer of y to point to the parent of x
        y->parent = x->parent; //make y the "top"/"root" now
        //Update the appropriate child 
        //pointer of x's parent to point to y
        if(x->parent == nullptr){ // if x was the root, now y is root
            root_ = y;
        }else if(x == x->parent->left){//if x is left subtree then y is now 
            x->parent->left = y; //left subtree of parent
        }else{
            x->parent->right = y;
        }
        //Update the left child of y to be x
        y->left = x;
        //Update the parent pointer of x to point to y
        x->parent = y;
    }
    /**
     * Right-rotate method described on p. 313 of CLRS.
     */
    void right_rotate(Node<K, V>* y) { //DONE
        /*
        y is originally "root"
        x is left child of y
        right child stays; right child of x becomes left
              y                  x
            /   \              /   \
           x     z    >>      a     y
          / \                     /   \
         a   b                   b     z
        */
        Node<K, V>* x = y->left;  // Save the left child of node y in a variable x
        y->left = x->right;  // Update the left child of node y to be the right child of x
        if (x->right != nullptr) {  // If there is a right child of x
            // Update its parent pointer to point to y
            x->right->parent = y;  // Updates the b to attach to y as left subchild
        }
        // Update the parent pointer of x to point to the parent of y
        x->parent = y->parent;  // Make x the "top"/"root" now
        // Update the appropriate child pointer of y's parent to point to x
        if (y->parent == nullptr) {  // If y was the root, now x is root
            root_ = x;
        } else if (y == y->parent->left) {  // If y is left subtree, then x is now left subtree of parent
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        // Update the right child of x to be y
        x->right = y;
        // Update the parent pointer of y to point to x
        y->parent = x;
    }


    /**
     * Returns the height of the red-black tree starting at node.
     * A null node starts at height -1.
     */
    int height(Node<K, V> *node) const {//DONE
        // Recurse down each subtree, then going up take the longest subtree and add one to get current height
        if(node == nullptr){
            return -1;
        }
        return 1 + max(height(node->left), height(node->right));
    }

    /**
     * Returns the count of leaves in the red-black tree starting at node.
     * For this method, a leaf is a non-null node that has no children.
     */
    size_t leaf_count(Node<K, V> *node) const {//DONE
        // If empty tree return
        if(node == nullptr){
            return 0;
        }else if(node->left == nullptr && node->right == nullptr){ // If leaf, return 1
            return 1;
        }else{
            return leaf_count(node->left) + leaf_count(node->right); // Add leaves of subtrees
        }
    }

    /**
     * Returns the count of internal nodes in the red-black tree starting at
     * node.
     * An internal node has at least one child.
     */
    size_t internal_node_count(Node<K, V> *node) const {//DONE
        // If empty, return 0
        if(node == nullptr){
            return 0;
        }else if(node->left == nullptr && node->right == nullptr){ // If leaf, return 0
            return 0;
        }else{
            return 1 + internal_node_count(node->left) + internal_node_count(node->right); // Add internal nodes of subtree and 1 for this node
        }
    }
    /**
     * Helper method to assist in the computation of tree diameter.
     */
    int diameter(Node<K, V> *node) const {
        /*
        The diameter of T’s left subtree.
        The diameter of T’s right subtree.
        The longest path between leaves that goes through the 
        root of T (this can be computed from the heights of the subtrees of T)
        */
        if(node == nullptr){
            return 0;
        }
        int lheight = height(node->left);
        int rheight = height(node->right);
 
        // get the diameter of left and right sub-trees
        int ldiameter = diameter(node->left);
        int rdiameter = diameter(node->right);
    
        // Return max of following three
        // 1) Diameter of left subtree
        // 2) Diameter of right subtree
        // 3) Height of left subtree + height of right subtree + 2
        return max(lheight + rheight + 2, max(ldiameter, rdiameter));
    }

    /**
     * Returns the width of the red-black tree at the designated level.
     * Width is defined as the number of nodes residing at a level.
     */
    size_t width(Node<K, V> *node, size_t level) const {//DONE
        // If empty tree or desired level is not reached, return 0
        if(node == nullptr){
            return 0;
        }else if(level == 0){ // If this is the level we want, return 1 for this node
            return 1;
        }
        return width(node->left, level-1) + width(node->right, level-1); // If this is not the level we want, look for the desired level in both subtrees and add
    }


    size_t null_count() const {
        return null_count(root_);
    }

    /**
     * Returns the count of null pointers in the red-black tree starting at
     * node.
     */
    size_t null_count(Node<K, V> *node) const {//DONE
        // If this is a null pointer return 1
        if(node == nullptr){
            return 1;
        }
        // Else find the null pointers in each subtree and add them
        return null_count(node->left) + null_count(node->right);
    }

    size_t sum_levels() const {
        return sum_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each non-null node in the red-black
     * tree starting at node.
     * For example, the tree
     *   5 <- level 0
     *  / \
     * 2   8 <- level 1
     *      \
     *       10 <- level 2
     * has sum 1*0 + 2*1 + 1*2 = 4.
     */
    size_t sum_levels(Node<K, V> *node, size_t level) const { //DONE
        // Base case: once we've passed the tree (if any) return 0 for null pointer
        if(node == nullptr){
            return 0;
        }
        // Add the level plus the sum of levels in each subtree
        return level + sum_levels(node->left, level+1) + sum_levels(node->right, level+1);
    }

    size_t sum_null_levels() const {
        return sum_null_levels(root_, 0);
    }

    /**
     * Returns the sum of the levels of each null node in the red-black tree
     * starting at node.
     * For example, the tree
     *    5 <- level 0
     *   / \
     *  2   8 <- level 1
     * / \ / \
     * * * * 10 <- level 2
     *       / \
     *       * * <- level 3
     * has sum 3(2) + 2(3) = 12.
     */
    size_t sum_null_levels(Node<K, V> *node, size_t level) const {//DONE
        // If this level is null, return the level
        if(node == nullptr){
            return level;
        }
        // Else look for null levels in each subtree and add
        return sum_null_levels(node->left, level+1) + sum_null_levels(node->right, level+1);
    }
};

#endif /* RBTREE_H_ */
