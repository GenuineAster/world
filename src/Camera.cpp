#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::getDirection() const {
	return -rotation * glm::vec3{0.f, 1.f, 0.f};
}

glm::vec3 Camera::getUp() const {
	return rotation * glm::vec3{0.f, 0.f, 1.f};
}

glm::vec3 Camera::getRight() const {
	return rotation * glm::vec3{1.f, 0.f, 0.f};
}

glm::mat4 Camera::getTransform() const {
	return glm::lookAt(target, target + getDirection(), getUp());
}
