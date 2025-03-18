/**
 * A fixed size array of elements where items are inserted sequentially.
 * If it's size is exceeded, it starts filling from the start, replacing
 * old elements.
 * Use cases: if we have a long running time series of data, this can
 * effeciently hold last few significant values.
 */

#pragma once

#include <cstdint>
#include <iostream>

namespace VG {

template <typename T>
class RingBuffer {
   public:
    RingBuffer() = delete;
    RingBuffer(uint16_t maxCapacity);
    ~RingBuffer();
    uint16_t GetCount();
    void Insert(T item);

   private:
    const uint16_t capacity;
    uint16_t startIndex;
    uint16_t count;
    T *buffer;
};

template <typename T>
RingBuffer<T>::RingBuffer(uint16_t maxCapacity) : capacity{maxCapacity} {
    startIndex = 0;
    count = 0;
    buffer = new T[capacity];
}

template <typename T>
VG::RingBuffer<T>::~RingBuffer() {
    delete[] buffer;
}

template <typename T>
uint16_t VG::RingBuffer<T>::GetCount() {
    return count;
}

template <typename T>
void VG::RingBuffer<T>::Insert(T item) {
    // Buffer not full.
    if (count < capacity) {
        startIndex = 0;
        buffer[count] = item;
        ++count;
    }
    // Buffer full.
    else {
        ++startIndex;
        if (startIndex >= capacity) {
            startIndex = 0;
            buffer[capacity - 1] = item;
        } else {
            buffer[startIndex - 1] = item;
        }
    }
}

}  // namespace VG