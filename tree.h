#pragma once

class Tree {
	
public:
	// Tree Attributes

	int type = 0;
	int treeHeight;
	int leavesWidth;
	int leavesHeight;

	std::vector<glm::vec3> leavesPoints;
	std::vector<glm::vec3> logPoints;

	glm::vec3 pos;

	Tree(glm::vec3 position, int height, int leaves_width, int leaves_height) {
		pos = position;
		treeHeight = height;
		leavesHeight = leaves_height;
		leavesWidth = leaves_width;

		std::cout << "Generating tree in: " << position.x << " " << position.z << std::endl;
	};

	void generate() {
		int startX = pos.x;
		int startZ = pos.z;
		int startY = pos.y;

		bool center = true;
		for (int y = startY + (treeHeight - leavesHeight); y < treeHeight + startY + 1; y++) {
			for (int i = -leavesWidth; i < leavesWidth + 1; i++) { // Z
				for (int i2 = -leavesWidth; i2 < leavesWidth + 1; i2++) { // X
					if (i == 0 && i2 == 0) if (!center) continue;
					leavesPoints.push_back(glm::vec3(startX + i2, y , startZ + i));
				}
			}
			// center = false;
		}

		for (int i = 0; i <= treeHeight - 1; i++) {
			logPoints.push_back(glm::vec3(startX, startY + i, startZ));
		}

		std::cout << "Tree generated" << std::endl;
	}

	glm::vec3 getPosition() {
		return pos;
	}

	std::vector<glm::vec3> getLeaves() {
		return leavesPoints;
	}

	std::vector<glm::vec3> getLogs() {
		return logPoints;
	}
};