#pragma once

class Texture {
public:
	unsigned int ID;
	int w, h, comp;

	Texture(const char* filename) {

		glGenTextures(1, &ID);

		glBindTexture(GL_TEXTURE_2D, ID);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_POINT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);


	};

	void bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};