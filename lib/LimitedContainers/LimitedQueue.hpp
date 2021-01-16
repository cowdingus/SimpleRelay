#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#ifdef NATIVE_UNIT_TEST
  #define F(x) x
#else
  #include <WString.h>
#endif

#include <assert.h>

template<typename E, size_t queueSize>
class LimitedQueue
{
public:
  LimitedQueue();
  LimitedQueue(const LimitedQueue& other);
  LimitedQueue& operator=(const LimitedQueue& other);

  void push(const E& e);
  void pop();
  void swap(LimitedQueue& other);

  E& front();
  E& back();
  const E& front() const;
  const E& back() const;

  size_t max_size() const;
  size_t size() const;
  bool empty() const;

private:
  E queue[queueSize];
  size_t occupied = 0;
};

template<typename E, size_t queueSize>
LimitedQueue<E, queueSize>::LimitedQueue()
{

}

template<typename E, size_t queueSize>
LimitedQueue<E, queueSize>::LimitedQueue(const LimitedQueue& other)
{
  memcpy(queue, other.queue, sizeof(E) * other.occupied);
  this->occupied = other.occupied;
}

template<typename E, size_t queueSize>
LimitedQueue<E, queueSize>& LimitedQueue<E, queueSize>::operator=(const LimitedQueue& other)
{
  memcpy(queue, other.queue, sizeof(E) * other.occupied);
  this->occupied = other.occupied;

  return *this;
}

template<typename E, size_t queueSize>
void LimitedQueue<E, queueSize>::push(const E& e)
{
  assert(occupied < size);

  queue[occupied] = e;
  ++occupied;
}

template<typename E, size_t queueSize>
void LimitedQueue<E, queueSize>::pop()
{
  assert(occupied != 0);

  memmove(queue, queue + 1, sizeof(E) * occupied - 1);
  --occupied;
}

template<typename E, size_t queueSize>
void LimitedQueue<E, queueSize>::swap(LimitedQueue& other)
{
  LimitedQueue<E, queueSize> tempOther(other);
  
  other = *this;
  this = tempOther;
}

template<typename E, size_t queueSize>
E& LimitedQueue<E, queueSize>::front()
{
  return queue[0];
}

template<typename E, size_t queueSize>
E& LimitedQueue<E, queueSize>::back()
{
  return queue[(occupied == 0) ? 0 : occupied - 1];
}

template<typename E, size_t queueSize>
const E& LimitedQueue<E, queueSize>::front() const
{
  return queue[0];
}

template<typename E, size_t queueSize>
const E& LimitedQueue<E, queueSize>::back() const
{
  return queue[(occupied == 0) ? 0 : occupied - 1];
}

template<typename E, size_t queueSize>
size_t LimitedQueue<E, queueSize>::max_size() const
{
  return queueSize;
}

template<typename E, size_t queueSize>
size_t LimitedQueue<E, queueSize>::size() const
{
  return occupied;
}

template<typename E, size_t queueSize>
bool LimitedQueue<E, queueSize>::empty() const
{
  return occupied == 0;
}