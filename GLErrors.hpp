#pragma once

#include <stdexcept>

namespace moe {
    class InitError : public std::runtime_error {
    public:
        InitError(const std::string& msg) : std::runtime_error(msg.c_str()) { };
    };
    class BufferError : public std::runtime_error {
    public:
        BufferError(const std::string& msg) : std::runtime_error(msg.c_str()) { };
    };
    class ShaderError : public std::runtime_error {
    public:
        ShaderError(const std::string& msg) : std::runtime_error(msg.c_str()) { };
    };
    class ShaderCompileError : public std::runtime_error {
    public:
        ShaderCompileError(const std::string& msg, const std::string& log) 
            :   std::runtime_error { msg.c_str() },
                log { log } 
                { };

        const std::string log;
    };
}