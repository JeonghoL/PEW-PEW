#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Floor {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class BuildObject
{
public:
	void generateFloor();

	const std::vector<Floor>& getFloorData() const { return floorwall; }
	size_t size() const { return floorwall.size(); }
	const Floor* data() const { return floorwall.data(); }
private:
	std::vector<Floor>floorwall;
};