#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <stdexcept>

namespace moe {

    // TODO: decide, errors in the same file or a dedicated file?
    class CameraError : public std::runtime_error {
    public:
        CameraError(const char* msg) : std::runtime_error{ msg } { }
    }; 

    // TODO: Interface for Camera, so a user could provide a customized camera ? (ie keep it inside a scene border)
    class Camera2D {
    public:
        Camera2D(glm::vec2 windowSize, glm::vec3 position = glm::vec3{ }, float rotation = 0, float zoom = 1);

        void update();
        void onWindowResize(glm::vec2 windowSize);  

        // returns the camera matrix, ie View Matrix multiplied with orthographic matrix
        glm::mat4 matrix() const    { return _o * _v; }
        glm::vec3 pos() const       { return _pos; }
        float zoom() const          { return _zoom; }
        float rotation() const      { return _rot; }

        void setPos(const glm::vec3& pos) { 
            _pos = pos; 
            _isMatrixDirty = true; 
            }
        void translate(const glm::vec3& alongLine) { 
            _pos += alongLine; 
            _isMatrixDirty = true; 
            }
        void zoomIn(float value) { 
            _zoom = std::min(_zoom + value, _maxZoom); 
            _isMatrixDirty = true; 
        }
        void zoomOut(float value) { 
            _zoom = std::max(_zoom - value, _minZoom);
            _isMatrixDirty = true;
            }
        void setZoom(float zoom) { 
            _zoom = std::min(std::max(zoom, _minZoom), _maxZoom); 
            _isMatrixDirty = true;
            }
        void rotate(float angle) {
            _rot += angle; // we do not care about degree bounds
            _isMatrixDirty = true;
        }
        

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

        // check if view matrix needs an update
        bool _isMatrixDirty;
    };
}