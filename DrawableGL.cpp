#include "DrawableGL.hpp"
#include "GLErrors.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace moe {

DrawableGL::DrawableGL(const std::vector<Vertex>& vertices, const glm::vec3& position) :
    vbo             { 0 },
    vao             { 0 },
    numVertices     { vertices.size() }
{
    glGenBuffers(1, &vbo);
    glCreateVertexArrays(1, &vao);
    if (vbo == 0 || vao == 0) {
        // TODO: Error handling
        throw BufferError("Failed to create GL-Buffer");
    } 
    transform.setPos(position);
    uploadVertices(vertices);
}

void DrawableGL::uploadVertices(const std::vector<Vertex>& vertices) {

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

/**
 * Draw assumes the shader is already on
*/
void DrawableGL::draw(const Shader& shader) const {

    // TODO: Catch errors
    // set model matrix
    int modelUniform = shader.uniform("M");
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(transform.worldMatrix()));

    // draw...
    glBindVertexArray(vao);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}
}