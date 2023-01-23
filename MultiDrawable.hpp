#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "DrawableGL.hpp"
#include "Transform.hpp"

namespace moe { 

    // TODO: Common interface with DrawableGL ??
    class MultiDrawableGL {
    public:
        // TODO: Do we need a position since we will position each object individually?
        // TODO later: Use game objects Transform instead of vec3?
        // TODO: Can we upload matrix arrays to the shader or do we need position, scale and rotation?
        //      ===> NO, glVertexAttribPointer supports 4 values max
        // TODO: Color should be defined per object, not per vertex
        MultiDrawableGL(
            const std::vector<Vertex>& vertices,
            const std::vector<glm::vec3>& positions, 
            const glm::vec3& position);

             // TODO: Destructor, free gl-resources

            void draw(const Shader& shader) const;
            Transform transform;

        protected:
            void uploadVertices(const std::vector<Vertex>& vertices, const std::vector<glm::vec3>& positions);

            const size_t vertexBufferIndex;
            const size_t matrixBufferIndex;

            // TODO: We may need to keep the matrices
            void uploadVerices(const std::vector<Vertex>& vertices);
            GLuint vbo[2];
            GLuint vao;
            GLsizei numVertices;
            GLsizei numObjects;
    };
}