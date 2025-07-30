#pragma once

#include <unistd.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <json/single_include/nlohmann/json.hpp>

#include "common_templates.h"

namespace common {
enum side { SIDE1 = 1, SIDE2 = 2, SIDE_ALL = 3 };
enum direction { UP = 1, DOWN = 2, NONE = 0 };

std::string getPathByFd(int fd);

std::string getSideString(side side_);

int writeAll(int fd, uint8_t *buf, int len);

int readAll(int fd, uint8_t *buf, int len);

// NOTE: работает только на неблокирующих дескрипторах
int readUntil(int fd, uint8_t *buf, int len, uint64_t time);

std::stringstream print(const uint8_t *pac, int len);

uint64_t timeMs();

std::string replace(const std::string &original, const std::string &old, const std::string &new_);

uint16_t crc16(const unsigned char *buf, unsigned int len);

uint16_t addCrc(uint16_t crc, uint8_t byte);

struct Buffer {
    std::vector<uint8_t> data;
    int size{0};

    Buffer(int capacity);
};

class IConfigurator {
  public:
    virtual ~IConfigurator() = default;
    virtual void configure(const nlohmann::json &config) = 0;
};

class Anchor {
    std::shared_ptr<lockable<bool>> anchored;

  public:
    class Hook {
        friend class Anchor;

        std::shared_ptr<lockable<bool>> anchored;

        Hook(std::shared_ptr<lockable<bool>> anchored) : anchored(anchored) {}

      public:
        Hook(const Hook &other) : anchored(other.anchored) {}

        const Hook &operator=(Hook other) {
            anchored.swap(other.anchored);

            return *this;
        }

        operator bool() const { return *anchored->lock(); }
    };

    Anchor() : anchored(common::make_lockable_ptr<bool>(true)) {}

    Anchor(const Anchor &) = delete;

    Anchor(const Anchor &&other) = delete;

    ~Anchor() { release(); }

    Hook hook() { return Hook(anchored); }

    void release() { anchored->store(new bool{false}); }
};
} // namespace common
