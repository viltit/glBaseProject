#include "Camera2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace moe {

    // TODO: Check parameters, ie. zoom > minZoom, rotation within 360 degrees, ...
    Camera2D::Camera2D(glm::vec2 windowSize, glm::vec3 position, float rotation, float zoom) 
        :   _pos { position },
            _rot { rotation },
            _zoom { zoom },
            _minZoom { 0.1 },
            _maxZoom { 100 },
            _sensitivity { 0.8 },
            // TODO: Fixing orthographic matrix to window size leads to a bigger visible scene on bigger windows
            // TODO: Good values for near and far plane
            _o { glm::ortho(-windowSize.x / 2.f, windowSize.x / 2.f, -windowSize.y / 2.f, windowSize.y / 2.f, -100.f, 100.f) }
    { }


    void Camera2D::update() {

        // TODO: Possible performance gain with a "dirty" flag, ie. only update matrix if position / zoom / rotation changed

        // TODO: We could smoothe the camera movement by storing the position change in the last update and using a 
        // fraction of it again -> this is where the sensitivity would come in

        _v = glm::mat4{ 1 };
        _v = glm::translate(_v, _pos);
        _v = glm::scale(_v, glm::vec3(_zoom, _zoom, _zoom));
        // TODO: Rotate camera, may be usefuel even in 2D
    }

    void Camera2D::onWindowResize(glm::vec2 windowSize) {
        _o = glm::ortho(-windowSize.x / 2.f, windowSize.x / 2.f, -windowSize.y / 2.f, windowSize.y / 2.f, -100.f, 100.f);
    }
}