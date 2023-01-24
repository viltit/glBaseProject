#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "DrawableGL.hpp"
#include "Transform.hpp"

namespace moe { 

    // TODO: Also look into glMultiDrawElementsIndirect

    // TODO: Common interface with DrawableGL ??
    class MultiDrawableGL {
    public:
        // TODO later: Use game objects Transform instead of matrices ?
        // TODO: How can we update the matrices and how costly is it ? (ie. glBufferSubData)
        // TODO: Color should be defined per object, not per vertex
        MultiDrawableGL(
            const std::vector<Vertex>& vertices,
            const std::vector<glm::mat4>& transformMatrices, 
            const glm::vec3& position);

             // TODO: Destructor, free gl-resources

            void draw(const Shader& shader) const;
            Transform transform;

        protected:
            void uploadVertices(const std::vector<Vertex>& vertices, const std::vector<glm::mat4>& transformMatrices);

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