#pragma once

class Cube {
	glm::vec3 pos;

	

public:
	// Cube Attributes
	
	int type = 0;
	unsigned int atlasID;

	Cube(glm::vec3 position) {
		pos = position;
	};

	glm::vec3 getPosition() {
		return pos;
	}
};