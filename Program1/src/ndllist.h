#ifndef NDLLIST_H
#define NDLLIST_H
#include <iostream>

using namespace std;

template <class Object>
struct LListNode {                  // a list node
  Object item;
  LListNode *next;
};

template <class Object>
class LList {
 public:
  LList( );                         // the constructor
  LList( const LList &rhs );        // the copy constructor
  ~LList( );                        // the destructor

  bool isEmpty( ) const;            // checks if a list is empty.
  int size( ) const;                // retrieves # list nodes;
  void clear( );                    // clean up all list entries.
  void insert( const Object &obj, const int index );// Insert a new object at data index `index` (0-based): index=0 means insert after the dummy header.
  int find( const Object &obj ) const;              // find the object position
  void remove( const Object &obj );                 // remove a given object
  const LList &operator=( const LList &rhs );       // assignment
  Object retrieve( const int index ) const;         // get an indexed object

 private:
  LListNode<Object> *header;                        // header
  LListNode<Object> *findByIndex( const int index ) const; // an indexed object
  LListNode<Object> *findPrevious( const Object &x ) const;// a previous object
};

#include "ndllist.cpp"
#endif
