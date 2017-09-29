#ifndef calcSTACK
#define calcSTACK

#include "calcError.hpp"
#include <algorithm>
#include <iostream>
#include <string>

template <typename Type> class calcStack {
  Type *start;
  Type *current;
  ulong size;
  ulong rate;
  bool accelerate;

public:
  calcStack();
  explicit calcStack(const ulong size);
  explicit calcStack(const bool);
  calcStack(const ulong, const ulong, const bool);
  calcStack(const calcStack &);
  ~calcStack();
  ulong totalElements() const;
  ulong capacity() const;
  bool setCapacity(const ulong);
  bool isEmpty() const;
  void beFast(const bool);
  void operator=(const calcStack &);
  void increaseSize();
  bool decreaseSize();
  bool find(const ulong, Type &) const;
  bool get(Type &) const;
  bool pop();
  bool pop(Type &);
  void push(const Type);
  void push(const Type *, const Type *);
  void reset();
  __attribute__((noinline)) void display(std::string before = "",
                                         std::string after = "") const;
};

template <typename Type> calcStack<Type>::calcStack() {
  this->size = 16;
  this->rate = 1;
  this->accelerate = 1;
  this->start = new Type[this->size];
  this->current = 0;
}

template <typename Type> calcStack<Type>::calcStack(const ulong size) {
  this->size = size;
  this->rate = 1;
  this->accelerate = 1;
  this->start = new Type[this->size];
  this->current = 0;
}

template <typename Type> calcStack<Type>::calcStack(const bool accelerate) {
  this->size = 16;
  this->rate = 1;
  this->accelerate = accelerate;
  this->start = new Type[this->size];
  this->current = 0;
}

template <typename Type>
calcStack<Type>::calcStack(const ulong size, const ulong rate,
                           const bool accelerate) {
  this->size = size;
  this->rate = rate;
  this->accelerate = accelerate;
  this->start = new Type[this->size];
  this->current = 0;
}

template <typename Type> calcStack<Type>::calcStack(const calcStack<Type> &t) {
  this->size = t.size;
  this->rate = t.rate;
  this->accelerate = t.accelerate;
  this->start = new Type[this->size];
  this->current = t.current ? this->start + (t.current - t.start) : 0;
  std::copy(t.start, t.current, this->start);
}

template <typename Type> ulong calcStack<Type>::totalElements() const {
  return this->current ? this->current - this->start : 0;
}

template <typename Type> ulong calcStack<Type>::capacity() const {
  return this->size;
}

template <typename Type> bool calcStack<Type>::setCapacity(const ulong s) {
  try {
    Type *temp = new Type[s];
    size = s;
    if (current) {
      std::copy(start, current, temp);
      current = temp + (s > (ulong)(current - start) ? current - start : s);
    }
    delete[] this->start;
    start = temp;
    return 1;
  } catch (const std::bad_alloc &x) {
    error(memAlloc);
  }
}

template <typename Type> bool calcStack<Type>::isEmpty() const {
  return !current;
}

template <typename Type>
bool calcStack<Type>::find(const ulong pos, Type &x) const {
  if (current)
    if (pos)
      return pos <= (ulong)(current - start) ? (x = start[pos - 1], 1) : 0;
    else
      return x = *(current - 1), 1;
  else
    return 0;
}

template <typename Type>
void calcStack<Type>::operator=(const calcStack<Type> &t) {
  if (this->start)
    delete[] this->start;
  this->start = new Type[this->size = t.size];
  this->rate = t.rate;
  this->accelerate = t.accelerate;
  this->current = t.current ? this->start + (t.current - t.start) : 0;
  std::copy(t.start, t.current, this->start);
}

template <typename Type> calcStack<Type>::~calcStack() {
  this->reset();
  delete[] this->start;
}

template <typename Type> void calcStack<Type>::beFast(const bool acc) {
  this->accelerate = acc;
}

template <typename Type> bool calcStack<Type>::pop(Type &y) {
  if (current) {
    y = *(--current);
    current = start == current ? 0 : current;
    return 1;
  }
  return 0;
}

template <typename Type> bool calcStack<Type>::pop() {
  if (this->current) {
    if (this->start == --this->current)
      this->current = 0;
    return 1;
  }
  return 0;
}

template <typename Type> bool calcStack<Type>::get(Type &y) const {
  return current ? (y = *(current - 1), 1) : 0;
}

template <typename Type> void calcStack<Type>::push(const Type y) {
  if (current) {
    if ((ulong)(current - start) == size)
      this->increaseSize();
    *current = y;
  } else
    *(current = start) = y;
  ++current;
}

template <typename Type>
void calcStack<Type>::push(const Type *start, const Type *end) {
  while (this->size < end - start)
    this->increaseSize();
  if (this->current) {
    std::copy(start, end, this->current - 1);
    this->current += end - start;
  } else {
    std::copy(start, end, this->start);
    this->current = this->start + (end - start);
  }
}

template <typename Type> void calcStack<Type>::reset() { current = 0; }

template <typename Type> bool calcStack<Type>::decreaseSize() {
  try {
    Type *temp;
    if (accelerate) {
      if (rate - 1 > rate)
        error(outOfRange);
      temp = new Type[size / accelerate];
    } else {
      if (size - rate > size)
        error(outOfRange);
      temp = new Type[size - rate];
    }
    size = accelerate ? size / --rate : size - rate;
    if (current) {
      std::copy(start, start + size, temp);
      current = temp + size;
    }
    delete[] this->start;
    start = temp;
    return 1;
  } catch (const std::bad_alloc &x) {
    error(memAlloc);
  }
}

template <typename Type> void calcStack<Type>::increaseSize() {
  try {
    Type *temp =
        accelerate ? new Type[(rate + 1) * size] : new Type[rate + size];
    size = accelerate ? size + (++rate) : size + rate;
    if (current) {
      std::copy(start, current, temp);
      current = temp + (current - start);
    }
    delete[] this->start;
    start = temp;
  } catch (const std::bad_alloc &x) {
    error(memAlloc);
  }
}

template <typename Type>
void calcStack<Type>::display(const std::string before,
                              const std::string after) const {
  if (this->current)
    for (Type *t = this->start; t < this->current; ++t)
      std::cout << before << *t << after;
#ifdef DEBUG
  else
    std::cerr << "Error: Stack is empty" << std::endl
              << "Array Address: " << this->start << std::endl
              << "At '" << __FILE__ << "' on " << __LINE__ << std::endl;
#endif
}

#endif

/*
  Local Variables:
  mode: c++
  c-file-offset: 2
  eval: (clang-format-buffer)
  fill-column: 80
  End:
 */
