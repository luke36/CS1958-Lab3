#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
template <typename T> class vector {
  friend class iterator;
  friend class const_iterator;

public:
  class const_iterator;
  class iterator {
    friend class vector<T>;

  private:
    vector<T> *vect;
    size_t pos;

    iterator(vector<T> *v, size_t p) : vect(v), pos(p) {}

  public:
    iterator() = default;

    iterator operator+(const size_t &n) const {
      return iterator(vect, pos + n);
    }
    iterator operator-(const size_t &n) const {
      return iterator(vect, pos - n);
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

    T &operator*() const { return vect->store[pos]; }

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

    const_iterator(const vector<T> *v, size_t p) : vect(v), pos(p) {}

  public:
    const_iterator() = default;

    const_iterator operator+(const size_t &n) const {
      return const_iterator(vect, pos + n);
    }
    const_iterator operator-(const size_t &n) const {
      return const_iterator(vect, pos - n);
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

    const T &operator*() const { return vect->store[pos]; }

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
  const size_t default_capacity = 8;
  size_t _size;
  size_t capacity;
  T *store;

  void clean() {
    for (size_t i = 0; i < _size; i++)
      store[i].~T();
    operator delete[](store);
  }

  void resize(size_t new_capacity) {
    capacity = new_capacity;
    T *new_store = (T *)operator new[](sizeof(T) * new_capacity);
    for (size_t i = 0; i < _size; i++) {
      new (new_store + i) T(store[i]);
      store[i].~T();
    }
    operator delete[](store);
    store = new_store;
  }

public:
  vector()
      : capacity(default_capacity), _size(0),
        store((T *)operator new[](sizeof(T) * default_capacity)) {}
  vector(const vector &other)
      : capacity(other.capacity), _size(other._size),
        store((T *)operator new[](sizeof(T) * other.capacity)) {
    for (size_t i = 0; i < other._size; i++)
      new (store + i) T(other.store[i]);
  }
  ~vector() { clean(); }
  vector &operator=(const vector &other) {
    if (this != &other) {
      clean();
      capacity = other.capacity;
      _size = other._size;
      store = (T *)operator new[](sizeof(T) * capacity);
      for (size_t i = 0; i < other._size; i++)
        new (store + i) T(other.store[i]);
    }
    return *this;
  }

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

  T &operator[](const size_t &pos) { return at(pos); }
  const T &operator[](const size_t &pos) const { return at(pos); }

  const T &front() const {
    if (_size == 0)
      throw container_is_empty();
    return at(0);
  }

  const T &back() const {
    if (_size == 0)
      throw container_is_empty();
    return at(_size - 1);
  }

  iterator begin() { return iterator(this, 0); }
  const_iterator cbegin() const { return const_iterator(this, 0); }

  iterator end() { return iterator(this, _size); }
  const_iterator cend() const { return const_iterator(this, _size); }

  bool empty() const { return (_size == 0); }

  size_t size() const { return _size; }

  void clear() {
    clean();
    capacity = default_capacity;
    _size = 0;
    store = (T *)operator new[](sizeof(T) * default_capacity);
  }

  iterator insert(const size_t &ind, const T &value) {
    if (ind > _size)
      throw index_out_of_bound();
    if (_size == capacity)
      resize(capacity * 2);
    if (_size > ind) {
      for (size_t i = _size - 1; i >= ind + 1; i--)
        new (store + i + 1) T(store[i]);
      new (store + ind + 1) T(store[ind]);
    }
    new (store + ind) T(value);
    _size++;
    return iterator(this, ind);
  }

  iterator insert(iterator pos, const T &value) {
    return insert(pos.pos, value);
  }

  iterator erase(const size_t &ind) {
    if (ind >= _size)
      throw index_out_of_bound();
    store[ind].~T();
    for (size_t i = ind; i < _size - 1; i++)
      new (store + i) T(store[i + 1]);
    _size--;
    if (_size <= capacity / 4 && capacity >= 4 * default_capacity)
      resize(capacity / 4);
    return iterator(this, ind);
  }

  iterator erase(iterator pos) { return erase(pos.pos); }

  void push_back(const T &value) { insert(_size, value); }

  void pop_back() {
    if (_size == 0)
      throw container_is_empty();
    erase(_size - 1);
  }
};
} // namespace sjtu

#endif