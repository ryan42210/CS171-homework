#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//Do not use image bigger than 30kB. Spuare image is better
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Here you need to fill the loadTexture function, so that you can use it to load texture in main.             //
// width and height must be pow of 2                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	
	return textureID;
}