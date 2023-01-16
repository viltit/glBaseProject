#pragma once

#include <string>
#include <GL/glew.h>

namespace moe {

    class Shader {
    public:
        Shader(const std::string& vertexShader, const std::string& fragmentShader);
        ~Shader();

        void on() const;
        void off() const;

        int uniform(const std::string& name) const;

    private:

        void compile(const std::string& shader, GLuint shaderID);
        void link();

        GLuint _id;
        GLuint _vertexID;
        GLuint _fragmentID;
        GLuint _geoID;
        GLuint _tessID;

    };
}

