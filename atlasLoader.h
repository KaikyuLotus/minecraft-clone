#pragma once

#define TINYPNG_IMPLEMENTATION

#include <vector>
#include "tinypng.h"



class atlasLoader {
	tpImage* loadedTpImages[20];
	const char* loadedTpImagesNames[20];

	int total = 0;
	int w = 48;
	int h = 48;

public:
	std::vector<unsigned int> textureIDs;

	atlasLoader() { }

	void freeTextures() {
		for (int i = 0; i < total; i++) {
			//free(loadedTpImages[i].pix);
			// memset(loadedTpImages[i], 0, sizeof(loadedTpImages[i]));
		}
	}

	tpImage loadTexture(const char *fileName) {
		tpImage loadedPNG;

		int i;

		if (total > 0) {
			for (i = 0; i < total; i++) {
				std::cout << "Checking if " << fileName << " already exists..." << std::endl;
				if (strcmp(fileName, loadedTpImagesNames[i]) == 0)
					return *loadedTpImages[i];
			}
		}

		std::cout << "First loading: " << fileName << "..." << std::endl;

		

		loadedPNG = tpLoadPNG(fileName);
		// loadedTpImagesNames[i + 1] = fileName;
		// loadedTpImages[i + 1] = &loadedPNG;
		// total += 1;

		

		return loadedPNG;
	}

	unsigned int makeTexture(tpImage atlasPointer) {
		unsigned int ID;

		// tpSavePNG("atlas.png", &atlasPointer);

		glGenTextures(1, &ID);

		glBindTexture(GL_TEXTURE_2D, ID);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasPointer.pix);
		// glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Atlas with ID " << ID << " made." << std::endl;

		return ID;
	}

	void loadAll() {
		std::cout << "Loading atlas..." << std::endl;
		tpImage* pngs[8];
		unsigned int currentID;
		std::vector<tpImage> currentAtlasTextures;
		for (int i = 0; i < 7; i++) {
			for (unsigned int i2 = 0; i2 < 6; i2++) {
				currentAtlasTextures.push_back(loadTexture(cubesTextures[i][i2]));
			}
			for (unsigned int i3 = 0; i3 < currentAtlasTextures.size(); i3++) {
				pngs[i3] = &currentAtlasTextures[i3];
			}


			tpAtlasImage* imgs_out = (tpAtlasImage*)malloc(sizeof(tpAtlasImage) * 6);

			unsigned int sID = makeTexture(tpMakeAtlas(64, 64, *pngs, 6, imgs_out));

			textureIDs.push_back(sID);

			std::cout << sID << std::endl;

			currentAtlasTextures.clear();
			
		}



		// freeTextures();
	}
};