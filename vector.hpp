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
    friend class vector<T>;

  private:
    vector<T> *vect;
    int pos;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator operator+(const int &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos + n;
      return res;
    }
    iterator operator-(const int &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos - n;
      return res;
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      if (vect != rhs.vect)
        throw invalid_iterator();
      return pos - rhs.pos;
    }
    iterator &operator+=(const int &n) {
      pos += n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      pos -= n;
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) { return *this - 1; }
    /**
     * TODO ++iter
     */
    iterator &operator++() { return *this += 1; }
    /**
     * TODO iter--
     */
    iterator operator--(int) { return *this - 1; }
    /**
     * TODO --iter
     */
    iterator &operator--() { return *this -= 1; }
    /**
     * TODO *it
     */
    T &operator*() const { return vect->store[pos]; }
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    bool operator==(const const_iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator {
    friend class vector<T>;

  private:
    const vector<T> *vect;
    int pos;

  public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator operator+(const int &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos + n;
      return res;
    }
    iterator operator-(const int &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos - n;
      return res;
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      if (vect != rhs.vect)
        throw invalid_iterator();
      return pos - rhs.pos;
    }
    iterator &operator+=(const int &n) {
      pos += n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      pos -= n;
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) { return *this - 1; }
    /**
     * TODO ++iter
     */
    iterator &operator++() { return *this += 1; }
    /**
     * TODO iter--
     */
    iterator operator--(int) { return *this - 1; }
    /**
     * TODO --iter
     */
    iterator &operator--() { return *this -= 1; }
    /**
     * TODO *it
     */
    const T &operator*() const { return vect->store[pos]; }
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory address).
     */
    bool operator==(const iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    bool operator==(const const_iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO Constructs
   * Atleast two: default constructor, copy constructor
   */
private:
  iterator make_iterator(int pos) {
    iterator i;
    i.vect = this;
    i.pos = pos;
    return i;
  }

public:
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
    if (pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  const T &at(const size_t &pos) const {
    if (pos >= _size)
      throw index_out_of_bound();
    return store[pos];
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) { return at(pos); }
  const T &operator[](const size_t &pos) const { return at(pos); }
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
  iterator begin() {
    iterator i;
    i.vect = this;
    i.pos = 0;
    return i;
  }
  const_iterator cbegin() const {
    const_iterator i;
    i.vect = this;
    i.pos = 0;
    return i;
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() {
    iterator i;
    i.vect = this;
    i.pos = _size - 1;
    return i;
  }
  const_iterator cend() const {
    const_iterator i;
    i.vect = this;
    i.pos = _size - 1;
    return i;
  }
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
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > _size)
      throw index_out_of_bound();
    if (_size == capacity)
      resize(capacity * 2);
    for (int i = _size - 1; i >= ind; i--)
      store[i + 1] = store[i];
    store[ind] = value;
    _size++;
    return make_iterator(ind);
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const T &value) {
    return insert(pos.pos, value);
  }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind) {
    if (ind >= _size)
      throw index_out_of_bound();
    for (int i = ind; i < _size - 1; i++)
      store[i] = store[i + 1];
    _size--;
    if (_size <= capacity / 2)
      resize(capacity / 2);
    return make_iterator(ind);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) { return erase(pos.pos); }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) { insert(_size, value); }
  /**
   * remove the last element from the end.
   * throw container_is_empty if size() == 0
   */
  void pop_back() { erase(_size - 1); }
};

} // namespace sjtu

#endif