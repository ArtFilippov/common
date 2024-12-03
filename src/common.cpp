#include "common/common.h"

int common::writeAll(int fd, uint8_t *buf, int len) {
    int total = 0;
    int n{};

    while (total < len) {
        n = write(fd, buf + total, len - total);
        if (n == -1 || n == 0) {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

int common::readAll(int fd, uint8_t *buf, int len) {
    int total = 0;
    int n{};

    while (total < len) {
        n = read(fd, buf + total, len - total);
        if (n == -1 || n == 0) {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

int common::readUntil(int fd, uint8_t *buf, int len, uint64_t time) {
    int total = 0;
    int n{};

    while (total < len &&
           std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                   .count() < time) {
        n = read(fd, buf + total, len - total);
        if (n == -1) {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

uint64_t common::timeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
        .count();
}

std::stringstream common::print(uint8_t *pac, int len) {
    std::stringstream buf;
    buf << "[" << std::hex;
    for (int i = 0; i < len - 1; ++i) {
        buf << "0x" << (int) pac[i] << ", ";
    }
    buf << "0x" << (int) pac[len - 1] << "]";

    return buf;
}

std::string common::getSideString(side side_) {
    switch (side_) {
    case SIDE1:
        return "SIDE1";
    case SIDE2:
        return "SIDE2";
    case SIDE_ALL:
        return "SIDE_ALL";
    default:
        return "INVALID_SIDE";
    }
}
