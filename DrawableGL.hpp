#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#include "Shader.hpp"
#include "Transform.hpp"

namespace moe {

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
    };

    /**
     * TODO: Graphics-API-Independent Interface Drawable ??
     * TODO: Use one drawable for several objects.
     * TODO: Some sort of cache for vertex buffers.
    */
    class DrawableGL {
    public:

        DrawableGL(const std::vector<Vertex>& vertices, const glm::vec3& position);
        void draw(const Shader& shader) const;

        Transform transform;

    protected:
        void uploadVerices(const std::vector<Vertex>& vertices);

        GLuint vbo;
        GLuint vao;
        GLsizei numVertices;
    };

    class Triangle : public DrawableGL {
    public:

        Triangle(const glm::vec4& color, const glm::vec3& position = glm::vec3{ })
            : DrawableGL { 
                std::vector<Vertex>  {
                    Vertex { glm::vec3(-100, -100, 0.0), color },
                    Vertex { glm::vec3(100, -100, 0.0), color },
                    Vertex { glm::vec3(0.0,  100, 0.0), color } },
                position }
        { }
    };



    class Rectangle : public DrawableGL {
    public:
        Rectangle(const glm::vec4& color, const glm::vec3& position = glm::vec3{ }) 
            : DrawableGL { 
                std::vector<Vertex> {
                    Vertex{ glm::vec3(-100, -100, 0.0), color },
                    Vertex{ glm::vec3(100, -100, 0.0), color },
                    Vertex{ glm::vec3(-100,  100, 0.0), color },
                    Vertex{ glm::vec3(-100,  100, 0.0), color },
                    Vertex{ glm::vec3(100, -100, 0.0), color },
                    Vertex{ glm::vec3(100, 100, 0.0), color } },
                position }
        { }
    };
}