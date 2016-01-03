#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	glm::vec3 position;
	glm::quat rotation;

	glm::vec3 getDirection() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;
	glm::mat4 getTransform() const;
	glm::mat4 getModulatedTransform(const glm::vec2 modulator) const;
	glm::tvec2<int16_t> getPositionTileIndex(const glm::vec2 modulator) const;
};
