#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vertex.hpp"

using namespace std;

class Texture {
public:
    Texture() = default;
    Texture(const char *path);

    void genTexture();

    inline unsigned int getTexture() {
        return this->textureID;
    }

private:
    unsigned char *textureData;
    unsigned int textureID;
    int width, height, nrChannels;
    const char *path;
};

#endif