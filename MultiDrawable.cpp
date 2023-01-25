#include "MultiDrawable.hpp"

#include "GLErrors.hpp"

namespace moe {

    MultiDrawableGL::MultiDrawableGL(
        const std::vector<Vertex>& vertices,
        const std::vector<glm::mat4>& transformMatrices, 
        const std::vector<glm::vec4>& colors,
        const glm::vec3& position) 
        
        :   vertexBufferIndex { 0 },
            matrixBufferIndex { 1 },
            colorBufferIndex  { 2 },
            vbo             { 0, 0, 0 },
            vao             { 0 },
            numVertices     { vertices.size() },
            numObjects      { transformMatrices.size() }
        {
            glGenBuffers(3, &vbo[0]);
            glCreateVertexArrays(1, &vao);
            if (vbo[0] == 0 || vbo[1] == 0 || vbo[2] == 0 || vao == 0) {
                throw BufferError("Failed to create GL-Buffer");
            } 
            transform.setPos(position);
            uploadVertices(vertices, transformMatrices, colors);
        }

        MultiDrawableGL::~MultiDrawableGL() {
            glDeleteBuffers(3, &vbo[0]);
            glDeleteVertexArrays(1, &vao);
        }

        void MultiDrawableGL::update(const std::vector<glm::mat4>& positions) {
            if (positions.size() > numObjects) {
                throw BufferError("Trying to buffer more data than buffer is capable of.");
            }
            glBindBuffer(GL_ARRAY_BUFFER, vbo[matrixBufferIndex]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * positions.size(), positions.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void MultiDrawableGL::draw(const Shader& shader) const {
            // TODO: Catch errors
            // set model matrix
            int modelUniform = shader.uniform("M");
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(transform.worldMatrix()));

            // draw...
            glBindVertexArray(vao);
            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArraysInstanced(GL_TRIANGLES, 0, numVertices, numObjects);
            glBindVertexArray(0);
        }

        void MultiDrawableGL::uploadVertices(
            const std::vector<Vertex>& vertices, 
            const std::vector<glm::mat4>& transformMatrices,
            const std::vector<glm::vec4>& colors) {

            glBindVertexArray(vao);
            // vertices
            glBindBuffer(GL_ARRAY_BUFFER, vbo[vertexBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // TODO: Interleave colors and matrices            
            // matrices. We need to enable 4 Vertex Attributes because one Attribute can only hold 4 values
            size_t vec4Size = sizeof(glm::vec4);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[matrixBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, transformMatrices.size() * sizeof(glm::mat4), transformMatrices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, nullptr);
            glEnableVertexAttribArray(3); 
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
            glEnableVertexAttribArray(4); 
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(5); 
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

            glVertexAttribDivisor(2, 1);   // Important, tells the shader to only update this attribute per instance, not per vertex
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);

            // colors
            glBindBuffer(GL_ARRAY_BUFFER, vbo[colorBufferIndex]);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, nullptr);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }
}