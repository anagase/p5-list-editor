#ifndef LIST_HPP
#define LIST_HPP
/* List.hpp
 *
 * doubly-linked, double-ended list with Iterator interface
 * EECS 280 List/Editor Project
 */

#include <iostream>
#include <iterator> //std::bidirectional_iterator_tag
#include <cassert>  //assert
#include <cstddef>  //std::ptrdiff_t
#include <utility>  //std::swap


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  // Default constructor
  List()
    : first(nullptr), last(nullptr), count(0) { }

  // Copy constructor
  List(const List &other)
    : first(nullptr), last(nullptr), count(0) {
    copy_all(other);
  }

  // Assignment operator
  List& operator=(const List &other) {
    if (this != &other) {
      clear();
      copy_all(other);
    }
    return *this;
  }

  // Destructor
  ~List() {
    clear();
  }

  //EFFECTS:  returns true if the list is empty
  bool empty() const {
    return count == 0;
  }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow. Instead, keep track of the size
  //         with a private member variable. That's how std::list does it.
  int size() const {
    return count;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front() {
    assert(!empty());
    return first->datum;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back() {
    assert(!empty());
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum) {
    Node *n = new Node;
    n->datum = datum;
    n->prev = nullptr;
    n->next = first;
    if (first) {
      first->prev = n;
    } else { // was empty
      last = n;
    }
    first = n;
    ++count;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum) {
    Node *n = new Node;
    n->datum = datum;
    n->next = nullptr;
    n->prev = last;
    if (last) {
      last->next = n;
    } else { // was empty
      first = n;
    }
    last = n;
    ++count;
  }

  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the front of the list
  void pop_front() {
    assert(!empty());
    Node *n = first;
    first = first->next;
    if (first) {
      first->prev = nullptr;
    } else {
      last = nullptr;
    }
    delete n;
    --count;
  }

  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the back of the list
  void pop_back() {
    assert(!empty());
    Node *n = last;
    last = last->prev;
    if (last) {
      last->next = nullptr;
    } else {
      first = nullptr;
    }
    delete n;
    --count;
  }

  //MODIFIES: invalidates all iterators to the removed elements
  //EFFECTS:  removes all items from the list
  void clear() {
    Node *cur = first;
    while (cur) {
      Node *next = cur->next;
      delete cur;
      cur = next;
    }
    first = nullptr;
    last = nullptr;
    count = 0;
  }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you should omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists.

private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other) {
    // requires this is empty
    assert(first == nullptr && last == nullptr && count == 0);
    for (Node *p = other.first; p != nullptr; p = p->next) {
      push_back(p->datum);
    }
  }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty
  int count = 0;

public:
  ////////////////////////////////////////
  friend class Iterator;
  class Iterator {
  public:
    //OVERVIEW: Iterator interface to List

    // Default constructor
    Iterator()
      : list_ptr(nullptr), node_ptr(nullptr) { }

    // Type aliases required to work with STL algorithms. Do not modify these.
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // REQUIRES: Iterator is dereferenceable
    // EFFECTS: returns the underlying element by pointer
    // NOTE: T must be a class type to use the syntax it->. If T has a
    //       member variable f, then it->f accesses f on the
    //       underlying T element.
    T* operator->() const {
      assert(list_ptr);
      assert(node_ptr);
      return &(node_ptr->datum);
    }

    // Dereference operator
    T& operator*() const {
      assert(list_ptr);
      assert(node_ptr);
      return node_ptr->datum;
    }

    // Prefix ++
    Iterator& operator++() {
      assert(list_ptr);
      assert(node_ptr); // incrementing end is undefined
      node_ptr = node_ptr->next;
      return *this;
    }

    // Postfix ++
    Iterator operator++(int /*dummy*/) {
      Iterator copy = *this;
      operator++();
      return copy;
    }

    // Prefix -- (provided earlier)
    Iterator& operator--() { // prefix -- (e.g. --it)
      assert(list_ptr);
      assert(*this != list_ptr->begin());
      if (node_ptr) {
        node_ptr = node_ptr->prev;
      } else { // decrementing an end Iterator moves it to the last element
        node_ptr = list_ptr->last;
      }
      return *this;
    }

    // Postfix -- (provided earlier)
    Iterator operator--(int /*dummy*/) { // postfix -- (e.g. it--)
      Iterator copy = *this;
      operator--();
      return copy;
    }

    // Equality operators
    bool operator==(const Iterator &other) const {
      return list_ptr == other.list_ptr && node_ptr == other.node_ptr;
    }
    bool operator!=(const Iterator &other) const {
      return !(*this == other);
    }

  private:
    const List *list_ptr; //pointer to the List associated with this Iterator
    Node *node_ptr; //current Iterator position is a List node

    // allow List to access Iterator internals
    friend class List<T>;

    // construct an Iterator at a specific position in the given List
    Iterator(const List *lp, Node *np)
      : list_ptr(lp), node_ptr(np) { }

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(this, first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const {
    return Iterator(this, nullptr);
  }

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS: Removes a single element from the list container.
  //         Returns An iterator pointing to the element that followed the
  //         element erased by the function call
  Iterator erase(Iterator i) {
    assert(i.list_ptr == this);
    assert(i.node_ptr); // must be dereferenceable
    Node *n = i.node_ptr;
    Node *next = n->next;

    if (n->prev) {
      n->prev->next = n->next;
    } else {
      // removing first
      first = n->next;
    }

    if (n->next) {
      n->next->prev = n->prev;
    } else {
      // removing last
      last = n->prev;
    }

    delete n;
    --count;
    return Iterator(this, next);
  }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: Inserts datum before the element at the specified position.
  //         Returns an iterator to the the newly inserted element.
  Iterator insert(Iterator i, const T &datum) {
    assert(i.list_ptr == this);
    // Inserting before end() -> push_back
    if (i.node_ptr == nullptr) {
      push_back(datum);
      return Iterator(this, last);
    }

    Node *cur = i.node_ptr;
    Node *n = new Node;
    n->datum = datum;
    n->next = cur;
    n->prev = cur->prev;

    if (cur->prev) {
      cur->prev->next = n;
    } else {
      // inserting at front
      first = n;
    }
    cur->prev = n;
    ++count;
    return Iterator(this, n);
  }

};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed. Do add the public member functions for
// Iterator.


#endif // Do not remove this. Write all your code above this line.

