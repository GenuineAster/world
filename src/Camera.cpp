#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::vec2 cam_mod(const glm::vec2 a, const glm::vec2 b) {
	auto x = a.x - b.x * int(a.x/b.x);
	auto y = a.y - b.y * int(a.y/b.y);
	return {x, y};
}

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
	return glm::lookAt(position, position + getDirection(), getUp());
}

glm::mat4 Camera::getModulatedTransform(const glm::vec2 modulator) const {
	auto tmp = glm::vec3(cam_mod(glm::vec2(position), modulator), position.z);
	return glm::lookAt(tmp, tmp + getDirection(), getUp());
}

glm::tvec2<int16_t> Camera::getPositionTileIndex(const glm::vec2 modulator) const {
	return {glm::vec2(position) / modulator};
}
