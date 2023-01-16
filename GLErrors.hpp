#pragma once

#include <stdexcept>

namespace moe {
    class InitError : public std::runtime_error {
    public:
        InitError(const char* msg) : std::runtime_error(msg) { };
    };
    class BufferError : public std::runtime_error {
    public:
        BufferError(const char* msg) : std::runtime_error(msg) { };
    };
}