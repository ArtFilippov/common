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

std::stringstream print(const uint8_t *pac, std::size_t len);

uint64_t timeMs();

std::string replace(const std::string &original, const std::string &old, const std::string &new_);

std::size_t linesOf(const std::string &str);

std::optional<int> stoi(const std::string &str, int base = 10);

uint16_t crc16(const unsigned char *buf, unsigned int len);

uint16_t addCrc(uint16_t crc, uint8_t byte);

class IConfigurator {
  public:
    virtual ~IConfigurator() = default;
    virtual void configure(const nlohmann::json &config) = 0;
};

class Anchor {
    std::shared_ptr<std::atomic_bool> anchored;

  public:
    class Hook {
        friend class Anchor;

        std::shared_ptr<std::atomic_bool> anchored;

        Hook(std::shared_ptr<std::atomic_bool> anchored) : anchored(anchored) {}

      public:
        Hook(const Hook &other) : anchored(other.anchored) {}

        const Hook &operator=(Hook other) {
            anchored.swap(other.anchored);

            return *this;
        }

        operator bool() const { return anchored->load(std::memory_order::acquire); }
    };

    Anchor() : anchored(std::make_shared<std::atomic_bool>(true)) {}

    Anchor(const Anchor &) = delete;

    Anchor(const Anchor &&other) = delete;

    ~Anchor() { release(); }

    Hook hook() { return Hook(anchored); }

    void release() { anchored->store(false, std::memory_order::release); }
};
} // namespace common
