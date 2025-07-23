#pragma once
#include <atomic>
#include <exception>
#include <iostream>

namespace common {
template <typename T> class array_atomic {
    int size;
    std::atomic<T> *data;

  public:
    array_atomic(int size, T value) : size(size) {
        data = new std::atomic<T>[size];
        for (int i = 0; i < size; ++i) {
            data[i].store(value);
        }
    }

    array_atomic(const array_atomic &) = delete;

    ~array_atomic() { delete[] data; }

    std::atomic<T> &operator[](int i) { return data[i % size]; }

    int getSize() { return size; }
};

template <typename Int> void setBigEndian(uint8_t *pac, Int x) {
    for (int i = sizeof(x) - 1; i >= 0; --i) {
        pac[i] = x & 0xFF;
        x >>= __CHAR_BIT__;
    }
}

template <typename Int> Int getBigEndian(uint8_t *pac) {
    Int ans = 0;
    for (int i = 0; i < sizeof(ans); ++i) {
        ans <<= __CHAR_BIT__;
        ans += pac[i];
    }

    return ans;
}

template <typename Int> void setLittleEndian(uint8_t *pac, Int x) {
    for (int i = 0; i < sizeof(x); ++i) {
        pac[i] = x & 0xFF;
        x >>= __CHAR_BIT__;
    }
}

template <typename Int> Int getLittleEndian(uint8_t *pac) {
    Int ans = 0;
    for (int i = sizeof(ans) - 1; i >= 0; --i) {
        ans <<= __CHAR_BIT__;
        ans += pac[i];
    }

    return ans;
}

template <typename Int> Int toGrayCode(Int x) {
    Int ans = 0;
    for (int i = 0; i < sizeof(x); ++i) {
        uint8_t byte = x & 0xFF;
        uint8_t grey = byte ^ (byte >> 1);
        ans |= grey << (i * __CHAR_BIT__);
        x >>= __CHAR_BIT__;
    }

    return ans;
}

template <typename Int> void setAtomicBits(std::atomic<Int> &atomic, Int bits) {
    Int current = 0;

    Int newValue = current | bits;

    while (!atomic.compare_exchange_strong(current, newValue)) {
        newValue = current | bits;
    }
}

template <typename Int> void unsetAtomicBits(std::atomic<Int> &atomic, Int bits) {
    Int current = ~0;

    Int newValue = current & (~bits);

    while (!atomic.compare_exchange_strong(current, newValue)) {
        newValue = current & (~bits);
    }
}

template <typename Ptr> class no_null_ptr {
    Ptr p;

  public:
    no_null_ptr(Ptr p) {
        if (p == nullptr) {
            std::cerr << "terminate after providing nullptr in no_null_ptr constructor" << std::endl;
            std::terminate();
        }
    }
};

} // namespace common
