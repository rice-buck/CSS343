//Default constructor, create a dummy header.
template<class Object>
LList<Object>::LList( ) {                
  header = nullptr;              
}

//Copy constructor, create a dummy header and then assign rhs to this. Using the operator = defined below.
template<class Object>
LList<Object>::LList( const LList &rhs ) {
  header = new LListNode<Object>;               
  header->item = rhs.header->item;          //copy first node 
  header = nullptr;                           
  *this = rhs;                              // then assign rhs to this.
}

//Destructor, clear the list and then delete the dummy header.
template<class Object>
LList<Object>::~LList( ) {
  clear( );                                       // delete items except the dummy header.
  delete header;                                 // delete the dummy header.
}

// Check if the list is empty by checking if the next pointer of the dummy header is nullptr.
template<class Object>
bool LList<Object>::isEmpty( ) const {
  return header == nullptr;
}

// Return the number of nodes in the list by traversing the list and counting the nodes until reaching the end (nullptr).
template<class Object>
int LList<Object>::size( ) const {
  LListNode<Object> *p = header;
  int size = 0;

  while(p != nullptr){
    size++;
    p = p -> next;
  }               
  return size;                    
}


// Clear the list by repeatedly finding and removing the first object until the list is empty.
template<class Object>
void LList<Object>::clear() {
    while (header != nullptr) {
        LListNode<Object>* nextNode = header->next;
        delete header;
        header = nextNode;
    }
    header = nullptr;
}



// Insert a new object at data index `index` (0-based).
// index == 0 inserts at the front of the list (after the dummy header).
// index == size() appends to the end.
// If index is out of range, the function does nothing.
template<class Object>
void LList<Object>::insert(const Object& obj, const int index) {
    if (index == 0){ //when at the inserting to an empty list
      LListNode<Object>* newPtr = new LListNode<Object>;
      newPtr->item = obj;
      newPtr->next = header;
      header = newPtr;
      return;
    }


    // Find the node *before* the insertion position.
    // For index == 0, this remains the dummy header.
    
    LListNode<Object>* temp = header;
    for(int i = 0; temp != nullptr && i < index - 1; i++){
      temp = temp->next;
    }
    if(temp != nullptr){
      LListNode<Object>* newNode = new LListNode<Object>;
      newNode->item = obj;
      newNode->next = temp->next;
      temp->next = newNode;
    }

    // LListNode<Object>* prev = header;
    // int i=0;
    // while(i<index && prev->next !=nullptr){
    //     prev = prev->next;
    //     ++i;
    // }
        
    // // Index beyond list size
    // if (i!=index) return;

    // LListNode<Object>* newPtr = new LListNode<Object>;
    // newPtr->item = obj;
    // newPtr->next = prev->next;
    // prev->next = newPtr;
}

// This is the find function. 
//It takes an object as input and returns the index of the first occurrence of that object in the list. 
//It traverses the list starting from the first node (after the dummy header) and compares each node's item with the given object. 
//If it finds a match, it returns the index of that node. 
//If it reaches the end of the list without finding a match, it returns -1 to indicate that the object is not found.
template<class Object>
int LList<Object>::find(const Object& obj) const {
    LListNode<Object>* p = header;
    int index = 0;

    while (p != nullptr) {
        if (p->item == obj)
            return index;
        p = p->next;
        ++index;
    }
    return -1;
}



// This is the remove function. It takes an object as input and removes the first occurrence of that object from the list.
// It first finds the previous node of the node containing the object to be removed using the findPrevious function. 
// If the previous node is nullptr, it means the object is not found in the list, so we return without doing anything. 
//Otherwise, we update the next pointer of the previous node to skip the node to be removed and point to the next node after it. 
//Finally, we deallocate the memory used by the node to be removed.
template<class Object>
void LList<Object>::remove( const Object &obj ) {
  if (header == nullptr) return;

  // Find the previous node of the node to be removed. If the previous node is nullptr, it means the object is not found in the list, so we return without doing anything.
  // LListNode<Object> *current = findPrevious( obj );
  // if (current->next == nullptr ) // obj not found
  //   return;
  if(header->item == obj){ //if node to delete is the head, move head and delete node
    LListNode<Object> *deleteNode = header;
    header = header->next;
    delete deleteNode;
    return;
  }
  LListNode<Object> *current = findPrevious( obj );
  if (current->next == nullptr){
  cout << "Object to remove not found" << endl; // obj not found
    return;
  } 
  LListNode<Object> *deleteNode = current->next; // fix pointers  
  current->next = current->next->next;
  delete deleteNode;     // deallocate
}


// This is the assignment operator. It is called when an already initialized list is assigned to another list.
template<class Object>
const LList<Object> &LList<Object>::operator=( const LList &rhs ) {
  if ( this != &rhs ) { // avoid self-assignment
    clear( );           // deallocate old items
    
    int index=0;
    for (LListNode<Object> *rnode = rhs.header; 
      rnode != nullptr;
      rnode = rnode->next, ++index )
      insert( rnode->item, index );
  }
  return *this;
}

// This is the retrieve function. It takes an index as input and returns the object at that index in the list. 
// It first finds the node at the given index using the findByIndex function. 
// If the node is not nullptr, it returns the item of that node. Otherwise, it returns a default-constructed object to indicate that the index is out of bounds.
template<class Object>
Object LList<Object>::retrieve( const int index ) const {
  LListNode<Object> *obj = findByIndex( index );
  return ( obj != nullptr ) ? obj->item : Object{};
}

// This is a helper function to find the node at a given index. It traverses the list starting from the dummy header and counts the nodes until it reaches the specified index. 
//If it finds the node at that index, it returns a pointer to that node. If it reaches the end of the list without finding the index, it returns nullptr to indicate that the index is out of bounds.
template<class Object>
LListNode<Object> *LList<Object>::findByIndex( const int index ) const {
  if ( index < 0 ) // verify index
    return nullptr;

  LListNode<Object> *p = header;
  for ( int i = 0; p != nullptr; p = p->next, ++i ){
    if ( i == index ) return p;
}
  
  return nullptr;
}

// This is a helper function to find the previous node of the node containing the given object. 
//It traverses the list starting from the dummy header and checks if the next node's item matches the given object. 
//If it finds a match, it returns the current node as the previous node. 
//If it reaches the end of the list without finding a match, it returns NULL.
template<class Object>
LListNode<Object>* LList<Object>::findPrevious(const Object& x) const {
    LListNode<Object>* p = header;

    while (p->next != nullptr && p->next->item != x)
        p = p->next;

    return p;  // p->next == nullptr means "not found"
}

