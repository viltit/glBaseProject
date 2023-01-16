#pragma once

#include <glm/glm.hpp>
#include <algorithm>

namespace moe {

    // TODO: Interface for Camera, so a user could provide a customized camera ? (ie keep it inside a scene border)
    class Camera2D {
    public:
        Camera2D(glm::vec2 windowSize, glm::vec3 position = glm::vec3{ }, float rotation = 0, float zoom = 1);

        void update();
        void onWindowResize(glm::vec2 windowSize);  

        // returns the camera matrix, ie View Matrix multiplied with orthographic matrix
        glm::mat4 matrix() const    { return _o * _v; }

        glm::vec3 pos() const               { return _pos; }
        void setPos(const glm::vec3& pos)   { _pos = pos; }
        void translate(const glm::vec3& alongLine) { _pos += alongLine; }
        float zoom() const                  { return _zoom; }
        void zoomIn(float value)            { _zoom = std::min(_zoom + value, _maxZoom); }
        void zoomOut(float value)           { _zoom = std::max(_zoom - value, _minZoom);}
        void setZoom(float zoom)            { _zoom = std::min(std::max(zoom, _minZoom), _maxZoom); }

    private:
        
        glm::vec3 _pos;
        float _rot;
        float _zoom;
        float _minZoom;
        float _maxZoom;
        float _sensitivity;

        // orthographic matrix:
        glm::mat4 _o;
        // view matrix:
        glm::mat4 _v;
    };
}