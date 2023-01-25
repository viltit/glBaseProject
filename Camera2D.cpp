#include "Camera2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace moe {

    Camera2D::Camera2D(glm::vec2 windowSize, glm::vec3 position, float rotation, float zoom) 
        :   _pos { position },
            _rot { rotation },
            _zoom { zoom },
            _minZoom { 0.03 },
            _maxZoom { 100 },
            _sensitivity { 0.8 },
            // TODO: Fixing orthographic matrix to window size leads to a bigger visible scene on bigger windows
            // TODO: Good values for near and far plane
            _o { glm::ortho(-windowSize.x / 2.f, windowSize.x / 2.f, -windowSize.y / 2.f, windowSize.y / 2.f, -100.f, 100.f) },
            _isMatrixDirty { true }
    {
        if (_zoom <= 0) {
            throw CameraError{ "Can not initialize camera with negative zoom." };
        }
        if (_minZoom >= _maxZoom) {
            throw CameraError{ "Can not inizialize camera with minZoom >= maxZoom" };
        }       
    }


    void Camera2D::update() {

        // TODO: We could smoothe the camera movement by storing the position change in the last update and using a 
        // fraction of it again -> this is where the sensitivity would come in
        if (_isMatrixDirty) {
            _v = glm::mat4{ 1 };
            _v = glm::rotate(_v, glm::radians(_rot), glm::vec3{ 0, 0, 1});
            _v = glm::translate(_v, _pos);
            _v = glm::scale(_v, glm::vec3{ _zoom, _zoom, _zoom });
            _isMatrixDirty = false;
        }
    }

    void Camera2D::onWindowResize(glm::vec2 windowSize) {
        _o = glm::ortho(-windowSize.x / 2.f, windowSize.x / 2.f, -windowSize.y / 2.f, windowSize.y / 2.f, -100.f, 100.f);
    }
}