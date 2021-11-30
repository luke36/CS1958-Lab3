#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <malloc.h>
namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T> class vector {
  friend class iterator;
  friend class const_iterator;

private:
  const int factor = 2;
  size_t _size;
  size_t capacity;
  T *store;
  void resize(int new_capacity) {
    capacity = new_capacity;
    T *new_store = malloc(sizeof(T) * new_capacity);
    for (int i = 0; i < _size; i++)
      new_store[i] = store[i];
    free(store);
    store = new_store;
  }

public:
  /**
   * TODO
   * a type for actions of the elements of a vector, and you should write
   *   a class named const_iterator with same interfaces.
   */
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */
  class const_iterator;
  class iterator {
  private:
    vector<T> *vect;
    int pos;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator operator+(const int &n) const {
      // TODO
    }
    iterator operator-(const int &n) const {
      // TODO
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      // TODO
    }
    iterator &operator+=(const int &n) {
      // TODO
    }
    iterator &operator-=(const int &n) {
      // TODO
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) {}
    /**
     * TODO ++iter
     */
    iterator &operator++() {}
    /**
     * TODO iter--
     */
    iterator operator--(int) {}
    /**
     * TODO --iter
     */
    iterator &operator--() {}
    /**
     * TODO *it
     */
    T &operator*() const {}
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const {}
    bool operator==(const const_iterator &rhs) const {}
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {}
    bool operator!=(const const_iterator &rhs) const {}
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator {};
  /**
   * TODO Constructs
   * Atleast two: default constructor, copy constructor
   */
  vector() : capacity(16), _size(0), store(malloc(sizeof(T) * 16)) {}
  vector(const vector &other)
      : capacity(other.capacity), _size(other._size),
        store(malloc(sizeof(T) * other.capacity)) {
    for (int i = 0; i < other._size; i++)
      store[i] = other.store[i];
  }
  /**
   * TODO Destructor
   */
  ~vector() { free(store); }
  /**
   * TODO Assignment operator
   */
  vector &operator=(const vector &other) {
    if (this != &store) {
      free(store);
      capacity = other.capacity;
      _size = other._size;
      store = malloc(sizeof(T) * capacity);
      for (int i = 0; i < other._size; i++)
        store[i] = other.store[i];
    }
    return *this;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos) {
    if (pos < 0 || pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  const T &at(const size_t &pos) const {
    if (pos < 0 || pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) {
    if (pos < 0 || pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  const T &operator[](const size_t &pos) const {
    if (pos < 0 || pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const {
    if (_size == 0)
      throw container_is_empty();
    return store[0];
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (_size == 0)
      throw container_is_empty();
    return store[_size - 1];
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() {}
  const_iterator cbegin() const {}
  /**
   * returns an iterator to the end.
   */
  iterator end() {}
  const_iterator cend() const {}
  /**
   * checks whether the container is empty
   */
  bool empty() const { return (_size == 0); }
  /**
   * returns the number of elements
   */
  size_t size() const { return _size; }
  /**
   * clears the contents
   */
  void clear() {
    capacity = 16;
    _size = 0;
    free(store);
    store = malloc(sizeof(T) * 16);
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const T &value) {}
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {}
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) {}
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind) {}
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    if (_size == capacity)
      resize(capacity * 2);
    store[_size++] = value;
  }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() {
    if (_size == 0)
      throw container_is_empty();
    _size--;
    if (_size <= capacity / 2)
      resize(capacity / 2);
  }
};

} // namespace sjtu

#endif