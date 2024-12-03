#pragma once
#include <unistd.h>
#include <chrono>
#include <sstream>

namespace common {
int writeAll(int fd, uint8_t *buf, int len);

int readAll(int fd, uint8_t *buf, int len);

// NOTE: работает только на неблокирующих дескрипторах
int readUntil(int fd, uint8_t *buf, int len, uint64_t time);

std::stringstream print(uint8_t *pac, int len);

uint64_t timeMs();

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
} // namespace common
