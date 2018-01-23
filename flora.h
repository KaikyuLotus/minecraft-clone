#pragma once

class Flora {
	glm::vec3 pos;

public:
	// Flora Attributes

	int type = 0;
	unsigned int textureID;


	Flora(glm::vec3 position) {
		pos = position;
	};

	glm::vec3 getPosition() {
		return pos;
	}
};