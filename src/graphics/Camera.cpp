#include "graphics/Camera.h"

namespace graphics {

    Camera::Camera() {
        this->position.x = 0;
        this->position.y = 0;
    }

    void Camera::SetPosition(const util::Vector2 &pos) {
        this->position = pos;
    }

    util::Vector2 Camera::GetPosition() {
        return this->position;
    }

}