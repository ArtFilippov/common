#include "common/common.h"
#include "common/common_templates.h"

std::string common::getPathByFd(int fd) {
    const int bufSize = 256;

    std::string path = "/proc/self/fd/" + std::to_string(fd);
    char *device = new char[bufSize];
    int res = readlink(path.c_str(), device, bufSize);

    if (res == -1) {
        throw std::runtime_error{"readlink error"};
    }

    device[res] = 0;

    std::string ans{device};

    delete[] device;

    return ans;
}

int common::writeAll(int fd, uint8_t *buf, int len) {
    int total = 0;
    int n{};

    while (total < len) {
        n = write(fd, buf + total, static_cast<std::size_t>(len - total));
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
        n = read(fd, buf + total, static_cast<std::size_t>(len - total));
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

    while (total < len && common::timeMs() < time) {
        n = read(fd, buf + total, static_cast<std::size_t>(len - total));
        if (n == -1) {
            break;
        }
        total += n;
    }

    return n == -1 ? -1 : total;
}

uint64_t common::timeMs() {
    return static_cast<std::size_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count());
}

std::stringstream common::print(const uint8_t *pac, std::size_t len) {
    std::stringstream buf;
    buf << "[" << std::hex;
    for (std::size_t i = 0; i < len; ++i) {
        buf << "0x" << static_cast<uint32_t>(pac[i]) << ", ";
    }
    buf << "]";

    return buf;
}

std::string common::getSideString(side side_) {
    switch (side_) {
    case SIDE1:
        return "Side1";
    case SIDE2:
        return "Sside2";
    case SIDE_ALL:
        return "SideAll";
    default:
        return "INVALID_SIDE";
    }
}

std::string common::replace(const std::string &original, const std::string &old, const std::string &new_) {

    std::string newString = "";

    size_t begin = 0;

    size_t pos = original.find(old, begin);

    while (pos != std::string::npos) {
        newString += original.substr(begin, pos - begin) + new_;

        begin = pos + old.size();

        pos = original.find(old, begin);
    }

    newString += original.substr(begin);

    return newString;
}

uint16_t common::crc16(const unsigned char *buf, unsigned int len) {
    static const uint16_t table[2] = {0x0000, 0xA001};
    uint16_t crc = 0xFFFF;
    unsigned int i = 0;
    char bit = 0;
    unsigned int x = 0;

    for (i = 0; i < len; i++) {
        crc ^= buf[i];

        for (bit = 0; bit < 8; bit++) {
            x = crc & 0x01;
            crc >>= 1;
            crc ^= table[x];
        }
    }
    return crc;
}

uint16_t common::addCrc(uint16_t crc, uint8_t byte) {
    uint16_t table[2] = {0x0000, 0xA001};

    crc ^= byte;

    for (int bit = 0; bit < 8; bit++) {
        uint16_t x = crc & 0x01;
        crc >>= 1;
        crc ^= table[x];
    }

    return crc;
}
