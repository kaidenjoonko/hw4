#ifndef AVLBST_H
#define AVLBST_H


#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"


struct KeyError { };
//test test 

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
   // Constructor/destructor.
   AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
   virtual ~AVLNode();


   // Getter/setter for the node's height.
   int8_t getBalance () const;
   void setBalance (int8_t balance);
   void updateBalance(int8_t diff);


   // Getters for parent, left, and right. These need to be redefined since they
   // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
   // for more information.
   virtual AVLNode<Key, Value>* getParent() const override;
   virtual AVLNode<Key, Value>* getLeft() const override;
   virtual AVLNode<Key, Value>* getRight() const override;


protected:
   int8_t balance_;    // effectively a signed char
};


/*
 -------------------------------------------------
 Begin implementations for the AVLNode class.
 -------------------------------------------------
*/


/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
   Node<Key, Value>(key, value, parent), balance_(0)
{


}


/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{


}


/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
   return balance_;
}


/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
   balance_ = balance;
}


/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
   balance_ += diff;
}


/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
   return static_cast<AVLNode<Key, Value>*>(this->parent_);
}


/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
   return static_cast<AVLNode<Key, Value>*>(this->left_);
}


/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
   return static_cast<AVLNode<Key, Value>*>(this->right_);
}




/*
 -----------------------------------------------
 End implementations for the AVLNode class.
 -----------------------------------------------
*/




template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
   virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
   virtual void remove(const Key& key);  // TODO
protected:
   virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);


   // Add helper functions here
   void insertHelper (AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
   void removeHelper(AVLNode<Key, Value>* n, int bDiff);
   void rotateLeft(AVLNode<Key, Value>* x);
   void rotateRight(AVLNode<Key, Value>* x);


};


/*
* Recall: If key is already in the tree, you should
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
   // TODO
   // extract key and value
   Key key = new_item.first;
   Value value = new_item.second;


   if(this->root_ == NULL){
     this->root_ = new AVLNode<Key, Value>(key, value, NULL);
     return;
   }


   // this section is same as bst insert
   Node<Key, Value>* curr = this->root_;
   Node<Key, Value>* parent = NULL;


   while(curr != NULL){
     parent = curr; // keep track of parent
     if(key < curr->getKey()){ // insert key is less than node's key
       curr = curr->getLeft();
     }
     else if(key >curr->getKey()) { // insert key is greater than node's key
       curr = curr->getRight();
     }
     else if(key == curr->getKey()){ // key exists
       curr->setValue(value); // just update the value
       return;
     }
   }


   // reached leaf node
   AVLNode<Key, Value>* insert = new AVLNode<Key, Value>(key, value, static_cast<AVLNode<Key, Value>* >(parent));
   if( key < parent->getKey()){ // insert key less than node's key
     parent->setLeft(insert);
   }
   else {
     parent->setRight(insert);
   }


   // balance after insertion
   AVLNode<Key, Value>* p = static_cast<AVLNode<Key, Value>* >(parent);
   insertHelper(p, insert);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertHelper (AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child) {
 if(parent == NULL){
   return;
 }
 if(parent->getLeft() == child) {
   parent->updateBalance(-1);
 }
 else {
   parent->updateBalance(1);
 }


 if(parent->getBalance() == 0) {
   return; // insertion didn't cause imbalance
 }
 else if( parent->getBalance() == 1 || parent->getBalance() == -1) {
   insertHelper(parent->getParent(), parent);
 }
 else if(parent->getBalance() == -2) { // left
   if(child->getBalance() == -1){
     rotateRight(parent);
     parent->setBalance(0);
     child->setBalance(0);
   }
   else {
     AVLNode<Key, Value>* grandchild = child->getRight();
     rotateLeft(child);
     rotateRight(parent);


     if(grandchild == NULL){
       return;
     }
    
     if(grandchild->getBalance() == 0) {
       parent->setBalance(0);
       child->setBalance(0);
     }
     else if(grandchild->getBalance() == 1) {
       parent->setBalance(0);
       child->setBalance(-1);
     }
     else {
       parent->setBalance(1);
       child->setBalance(0);
     }
     grandchild->setBalance(0);
   }
 }
 else if(parent->getBalance() == 2) { // right
   if(child->getBalance() == 1){
     rotateLeft(parent);
     parent->setBalance(0);
     child->setBalance(0);
   }
   else {
     AVLNode<Key, Value>* grandchild = child->getLeft();
     rotateRight(child);
     rotateLeft(parent);


     if(grandchild == NULL){
       return;
     }
    
     if(grandchild->getBalance() == 0) {
       parent->setBalance(0);
       child->setBalance(0);
     }
     else if(grandchild->getBalance() == -1) {
       parent->setBalance(0);
       child->setBalance(1);
     }
     else {
       parent->setBalance(-1);
       child->setBalance(0);
     }
     grandchild->setBalance(0);
   }
 }
}


/*
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
   // TODO
   AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
   if(n == NULL){
     return;
   }


   // if there are 2 children, swap with predecessor
   if(n->getLeft() != NULL && n->getRight()!= NULL) {
     AVLNode<Key, Value>* p = static_cast<AVLNode<Key,Value>*>(this->predecessor(n));
     nodeSwap(n, p);
   }


   // use left child unless there isn't one; use right child otherwise
   Node<Key, Value>* child = n->getLeft();
   if(n->getLeft() == NULL) {
     child = n->getRight();
   }


   AVLNode<Key, Value>* parent = n->getParent();
   AVLNode<Key, Value>* curr = n->getParent(); // tracks the parent of the node we removed
   int bDiff = 0;


   if(child != NULL) {
     child->setParent(parent);
   }


   if(parent == NULL) { // deleted the root
     this->root_ = child;
   }
   else if(parent->getLeft() == n){
     parent->setLeft(child);
     bDiff = 1;
     curr = parent;
   }
   else if(parent->getRight() == n) {
     parent->setRight(child);
     bDiff = -1;
     curr = parent;
   }


   delete n; // remove the node


   // rebalance
   removeHelper(curr, bDiff);


}


template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* n, int bDiff) {
 if(n == NULL){
   return;
 }


 AVLNode<Key, Value>* parent = n->getParent();
 int nDiff = 0;
 if(parent != NULL){
   if(parent->getLeft() == n) {
     nDiff = 1;
   }
   else {
     nDiff = -1;
   }
 }
 n->updateBalance(bDiff);


 if(n->getBalance() == 1 ||n->getBalance() == -1){ // height wasn't changed
   return;
 }
 else if( n->getBalance() == 0){
   removeHelper(parent, nDiff);
   return;
 }


 // need to rebalance here
 if(n->getBalance() == 2) {
   AVLNode<Key, Value> * rChild = n->getRight();


   int rBalance = 0;
   if(rChild != NULL) {
     rBalance = rChild->getBalance();
   }


   if(rBalance >= 0){  // right-right
     rotateLeft(n);
     if(rBalance == 0){
       n->setBalance(1);
       rChild->setBalance(-1);
       return; // height isn't changed
     }
     else {
       n->setBalance(0);
       rChild->setBalance(0);
       removeHelper(parent, nDiff);
     }
   }
   else { // right-left
     AVLNode<Key, Value>* grandchild = rChild->getLeft();
     int gBalance = 0;
     if(grandchild != NULL) {
       gBalance = grandchild->getBalance();
     }
     rotateRight(rChild);
     rotateLeft(n);


     if(gBalance == 0) {
       n->setBalance(0);
       rChild->setBalance(0);
     }
     else if(gBalance == -1){
       n->setBalance(0);
       rChild->setBalance(1);
     }
     else {
       n->setBalance(-1);
       rChild->setBalance(0);
     }
     if(grandchild != NULL) {
       grandchild->setBalance(0);
     }
     removeHelper(parent, nDiff);
   }
 }
 else if(n->getBalance() == -2) {
   AVLNode<Key, Value> * lChild = n->getLeft();


   int lBalance = 0;
   if(lChild != NULL) {
     lBalance = lChild->getBalance();
   }


   if(lBalance <= 0){  // left-left
     rotateRight(n);
     if(lBalance == 0){
       n->setBalance(-1);
       lChild->setBalance(1);
       return; // height isn't changed
     }
     else {
       n->setBalance(0);
       lChild->setBalance(0);
       removeHelper(parent, nDiff);
     }
   }
   else { // left-right
     AVLNode<Key, Value>* grandchild = lChild->getRight();
     int gBalance = 0;
     if(grandchild != NULL) {
       gBalance = grandchild->getBalance();
     }
     rotateLeft(lChild);
     rotateRight(n);


     if(gBalance == 0) {
       n->setBalance(0);
       lChild->setBalance(0);
     }
     else if(gBalance == 1){
       n->setBalance(0);
       lChild->setBalance(-1);
     }
     else {
       n->setBalance(1);
       lChild->setBalance(0);
     }
     if(grandchild != NULL) {
       grandchild->setBalance(0);
     }
     removeHelper(parent, nDiff); // remove requires checking for more imbalances
   }
 }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
   BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
   int8_t tempB = n1->getBalance();
   n1->setBalance(n2->getBalance());
   n2->setBalance(tempB);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x) {
 AVLNode<Key, Value>* y = x->getLeft();
 x->setLeft(y->getRight());


 if(y->getRight() != NULL) {
   y->getRight()->setParent(x);
 }


 y->setParent(x->getParent());


 if(x->getParent() == NULL) { // x was the root
   this->root_ = y;
 }
 else if( x->getParent()->getLeft() == x){ // x was left child
   x->getParent()->setLeft(y);
 }
 else {
   x->getParent()->setRight(y); // x was right child
 }


 y->setRight(x);
 x->setParent(y);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
 AVLNode<Key, Value>* y = x->getRight();
 x->setRight(y->getLeft());


 if(y->getLeft() != NULL) {
   y->getLeft()->setParent(x);
 }


 y->setParent(x->getParent());


 if(x->getParent() == NULL) { // x was the root
   this->root_ = y;
 }
 else if( x->getParent()->getLeft() == x){ // x was left child
   x->getParent()->setLeft(y);
 }
 else {
   x->getParent()->setRight(y); // x was right child
 }


 y->setLeft(x);
 x->setParent(y);
}


#endif



