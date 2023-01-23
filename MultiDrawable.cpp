#include "MultiDrawable.hpp"

#include "GLErrors.hpp"

namespace moe {

    MultiDrawableGL::MultiDrawableGL(
        const std::vector<Vertex>& vertices,
        const std::vector<glm::vec3>& positions, 
        const glm::vec3& position) 
        
        :   vertexBufferIndex { 0 },
            matrixBufferIndex { 0 },
            vbo             { 0, 0 },
            vao             { 0 },
            numVertices     { vertices.size() },
            numObjects      { positions.size() }
        {
            glGenBuffers(2, vbo);
            glCreateVertexArrays(1, &vao);
            if (vbo[0] == 0 || vbo[1] == 0 || vao == 0) {
                // TODO: Error handling
                throw BufferError("Failed to create GL-Buffer");
            } 
            transform.setPos(position);
            uploadVertices(vertices, positions);
        }

        void MultiDrawableGL::draw(const Shader& shader) const {
            // TODO
        }

        void MultiDrawableGL::uploadVertices(const std::vector<Vertex>& vertices, const std::vector<glm::vec3>& positions) {
            glBindVertexArray(vao);
            // vertices
            glBindBuffer(GL_ARRAY_BUFFER, vbo[vertexBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // matrices
            glBindBuffer(GL_ARRAY_BUFFER, vbo[matrixBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
            glVertexAttribDivisor(2, 1);   // Important, tells the shader to only update this attribute per instance, not per vertex
            
            glBindVertexArray(0);
        }
}