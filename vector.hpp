#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <malloc.h>
namespace sjtu {
template <typename T> class vector {
  friend class iterator;
  friend class const_iterator;

private:
  const size_t factor = 2;
  size_t _size;
  size_t capacity;
  T **store;
  void resize(size_t new_capacity) {
    capacity = new_capacity;
    T **new_store = (T **)malloc(sizeof(T *) * new_capacity);
    for (size_t i = 0; i < _size; i++) {
      new_store[i] = new T(*store[i]);
      delete store[i];
    }
    free(store);
    store = new_store;
  }

public:
  class const_iterator;
  class iterator {
    friend class vector<T>;

  private:
    vector<T> *vect;
    size_t pos;

  public:
    iterator operator+(const size_t &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos + n;
      return res;
    }
    iterator operator-(const size_t &n) const {
      iterator res;
      res.vect = vect;
      res.pos = pos - n;
      return res;
    }

    size_t operator-(const iterator &rhs) const {
      if (vect != rhs.vect)
        throw invalid_iterator();
      return pos - rhs.pos;
    }
    iterator &operator+=(const size_t &n) {
      pos += n;
      return *this;
    }
    iterator &operator-=(const size_t &n) {
      pos -= n;
      return *this;
    }

    iterator &operator++() { return *this += 1; }

    iterator operator++(int) {
      ++(*this);
      return *this - 1;
    }

    iterator &operator--() { return *this -= 1; }

    iterator operator--(int) {
      --(*this);
      return *this + 1;
    }

    T &operator*() const { return *(vect->store[pos]); }

    bool operator==(const iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    bool operator==(const const_iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }

    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  class const_iterator {
    friend class vector<T>;

  private:
    const vector<T> *vect;
    size_t pos;

  public:
    const_iterator operator+(const size_t &n) const {
      const_iterator res;
      res.vect = vect;
      res.pos = pos + n;
      return res;
    }
    const_iterator operator-(const size_t &n) const {
      const_iterator res;
      res.vect = vect;
      res.pos = pos - n;
      return res;
    }

    size_t operator-(const const_iterator &rhs) const {
      if (vect != rhs.vect)
        throw invalid_iterator();
      return pos - rhs.pos;
    }
    const_iterator &operator+=(const size_t &n) {
      pos += n;
      return *this;
    }
    const_iterator &operator-=(const size_t &n) {
      pos -= n;
      return *this;
    }

    const_iterator &operator++() { return *this += 1; }

    const_iterator operator++(int) {
      ++(*this);
      return *this - 1;
    }

    const_iterator &operator--() { return *this -= 1; }

    const_iterator operator--(int) {
      --(*this);
      return *this + 1;
    }
    const T &operator*() const { return *(vect->store[pos]); }

    bool operator==(const iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }
    bool operator==(const const_iterator &rhs) const {
      return vect == rhs.vect && pos == rhs.pos;
    }

    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };

private:
  iterator make_iterator(size_t pos) {
    iterator i;
    i.vect = this;
    i.pos = pos;
    return i;
  }

public:
  vector() : capacity(16), _size(0), store((T **)malloc(sizeof(T *) * 16)) {}
  vector(const vector &other)
      : capacity(other.capacity), _size(other._size),
        store((T **)malloc(sizeof(T *) * other.capacity)) {
    for (size_t i = 0; i < other._size; i++)
      store[i] = new T(*other.store[i]);
  }
  ~vector() {
    for (size_t i = 0; i < _size; i++)
      delete store[i];
    free(store);
  }
  vector &operator=(const vector &other) {
    if (this != &other) {
      for (size_t i = 0; i < _size; i++)
        delete store[i];
      free(store);
      capacity = other.capacity;
      _size = other._size;
      store = (T **)malloc(sizeof(T *) * capacity);
      for (size_t i = 0; i < other._size; i++)
        store[i] = new T(*other.store[i]);
    }
    return *this;
  }

  T &at(const size_t &pos) {
    if (pos >= _size)
      throw index_out_of_bound();
    return *store[pos];
  }
  const T &at(const size_t &pos) const {
    if (pos >= _size)
      throw index_out_of_bound();
    return *store[pos];
  }

  T &operator[](const size_t &pos) { return at(pos); }
  const T &operator[](const size_t &pos) const { return at(pos); }

  const T &front() const { return at(0); }

  const T &back() const { return at(_size - 1); }

  iterator begin() { return make_iterator(0); }
  const_iterator cbegin() const {
    const_iterator i;
    i.vect = this;
    i.pos = 0;
    return i;
  }

  iterator end() { return make_iterator(_size); }
  const_iterator cend() const {
    const_iterator i;
    i.vect = this;
    i.pos = _size;
    return i;
  }

  bool empty() const { return (_size == 0); }

  size_t size() const { return _size; }

  void clear() {
    capacity = 16;
    _size = 0;
    for (size_t i = 0; i < _size; i++)
      delete store[i];
    free(store);
    store = (T **)malloc(sizeof(T *) * 16);
  }

  iterator insert(const size_t &ind, const T &value) {
    if (ind > _size)
      throw index_out_of_bound();
    if (_size == capacity)
      resize(capacity * 2);
    if (_size > 0) {
      for (size_t i = _size - 1; i >= ind + 1; i--)
        store[i + 1] = store[i];
      store[ind + 1] = store[ind];
    }
    store[ind] = new T(value);
    _size++;
    return make_iterator(ind);
  }

  iterator insert(iterator pos, const T &value) {
    return insert(pos.pos, value);
  }

  iterator erase(const size_t &ind) {
    if (ind >= _size)
      throw index_out_of_bound();
    delete store[ind];
    if (_size > 0)
      for (size_t i = ind; i < _size - 1; i++)
        store[i] = store[i + 1];
    _size--;
    if (_size <= capacity / 2)
      resize(capacity / 2);
    return make_iterator(ind);
  }

  iterator erase(iterator pos) { return erase(pos.pos); }

  void push_back(const T &value) { insert(_size, value); }

  void pop_back() {
    if (_size > 0)
      erase(_size - 1);
  }
};

} // namespace sjtu

#endif