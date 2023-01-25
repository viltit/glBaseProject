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
        // TODO: Vertices do not need colors anymore
        // TODO: using translation vector and quaternion instead of matrix would reduce data by 50%  compared to a matrix
        MultiDrawableGL(
            const std::vector<Vertex>& vertices,
            const std::vector<glm::mat4>& transformMatrices, 
            const std::vector<glm::vec4>& colors,
            const glm::vec3& position);

             // TODO: Destructor, free gl-resources
            
            // TODO: wip testing, update ALL matrices every frame
            void update(const std::vector<glm::mat4>& positions);
            void draw(const Shader& shader) const;
            Transform transform;

        protected:
            void uploadVertices(const std::vector<Vertex>& vertices, const std::vector<glm::mat4>& transformMatrices, const std::vector<glm::vec4>& colors);

            const size_t vertexBufferIndex;
            const size_t matrixBufferIndex;
            const size_t colorBufferIndex;

            // TODO: We may need to keep the matrices
            void uploadVerices(const std::vector<Vertex>& vertices);
            GLuint vbo[3];
            GLuint vao;
            GLsizei numVertices;
            GLsizei numObjects;
    };
}