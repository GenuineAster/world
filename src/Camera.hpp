#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	glm::vec3 target;
	float distance;
	glm::quat rotation;

	glm::vec3 getDirection() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;
	glm::mat4 getTransform() const;
};
