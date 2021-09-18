#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "utils.hpp"

Texture::Texture()
    : width(0)
    , height(0)
{
}
Texture::Texture(std::string path)
    : width(0)
    , height(0)
{
    loadFromPath(path);
}

Texture::Texture(Eigen::Vector3f primary_rgb, Eigen::Vector3f secondary_rgb, unsigned int num_ckers, bool shifted)
    : width(0)
    , height(0)
{
    if (!shifted) generateCheckerboard(primary_rgb, secondary_rgb, num_ckers);
    else generateCheckerShifted(primary_rgb, secondary_rgb, num_ckers);
}

bool Texture::isNull() const
{
    return data.empty() || width == 0 || height == 0;
}

unsigned int Texture::getLevelNums() const
{
    return data.size();
}

Eigen::Vector2i Texture::getDimension() const
{
    return Eigen::Vector2i(width, height);
}

Eigen::Vector3f Texture::getPixel(float u, float v) const
{
    unsigned int x = (unsigned int)(u * width);
    unsigned int y = (unsigned int)(v * height);
    if (x == width) x--;
    if (y == height) y--;
    Eigen::Vector3f px_color;
    px_color <<
        data[0][(x + width * y) * 3],
        data[0][(x + width * y) * 3 + 1],
        data[0][(x + width * y) * 3 + 2];
    return px_color;
}

Eigen::Vector3f Texture::getPixel(float u, float v, unsigned int level) const
{
    Eigen::Vector3f px_color;
    unsigned int w, h;
    int scaler = (int)powf(2, level);
    w = (unsigned int)(width / scaler);
    h = (unsigned int)(height / scaler);
    float x = u * w;
    float y = v * h;
    Eigen::Vector2i x_id = findInterval(x, w);
    Eigen::Vector2i y_id = findInterval(y, h);

    unsigned int px1 = x_id[0] + w * y_id[0];
    unsigned int px2 = x_id[1] + w * y_id[0];
    unsigned int px3 = x_id[0] + w * y_id[1];
    unsigned int px4 = x_id[1] + w * y_id[1];
    float alpha_x = x - 0.5 - floorf(x - 0.5);
    float alpha_y = y - 0.5 - floorf(y - 0.5);

    for (int i = 0; i < 3; i++)
    {
        float color1 = (1.0f-alpha_x) * data[level][px1 * 3 + i] + alpha_x * data[level][px2 * 3 + i];
        float color2 = (1.0f-alpha_x) * data[level][px3 * 3 + i] + alpha_x * data[level][px4 * 3 + i];
        float color = (1.0f -alpha_y) * color1 + alpha_y * color2;
        px_color[i] = color;
    }


    /* take color by level */
    // Eigen::Vector3f px_color;
    // std::vector<Eigen::Vector3f> colorlist
    // {
    //     Eigen::Vector3f(1,0,0),
    //     Eigen::Vector3f(0,1,0),
    //     Eigen::Vector3f(0,0,1),
    // };
    // px_color = colorlist[level%3];
    
    return px_color;
}

void Texture::clear()
{
    width = 0;
    height = 0;
}
    
Texture::~Texture()
{
    clear();
}

void Texture::loadFromPath(std::string path)
{
    int w, h, n;
    unsigned char* raw_data = stbi_load(path.c_str(), &w, &h, &n, 0);
    data.resize(1);
    /* Only images with 1 or 3 channels are supported */
    if (raw_data && (n == 3 || n == 1))
    {
        // this->data = new float[w * h * 3];
        // this->data = std::make_shared<float[]>(w * h * 3);
        this->data[0].assign(w*h*3, 0);
        this->width = w;
        this->height = h;

        int ch_stride = n == 3 ? 1 : 0;
        for (int i = 0; i < w * h * n; i += n)
        {
            this->data[0][i] = (float)raw_data[i] / 255.0f;
            this->data[0][i + 1] = (float)raw_data[i + ch_stride] / 255.0f;
            this->data[0][i + 2] = (float)raw_data[i + ch_stride * 2] / 255.0f;
        }

        stbi_image_free(raw_data);
    }

    return;
}

void Texture::generateCheckerboard(Eigen::Vector3f primary_rgb, Eigen::Vector3f secondary_rgb, unsigned int num_ckers)
{
    int checker_size = 32;
    int w = num_ckers * checker_size, h = num_ckers * checker_size;
    Eigen::Vector3f color_list[2];
    color_list[0] = primary_rgb;
    color_list[1] = secondary_rgb;

    width = w;
    height = h;
    // data = new float[width * height * 3];
    data.resize(1);
    data[0].resize(width * height * 3);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Eigen::Vector3f color = color_list[((int)(x / checker_size) + (int)(y / checker_size)) % 2];
            data[0][(y * w + x) * 3] = color[0];
            data[0][(y * w + x) * 3 + 1] = color[1];
            data[0][(y * w + x) * 3 + 2] = color[2];
        }
    }

    return;
}

void Texture::generateCheckerShifted(Eigen::Vector3f primary_rgb, Eigen::Vector3f secondary_rgb, unsigned int num_ckers)
{
    int checker_size = 256;
    int w = num_ckers * checker_size, h = num_ckers * checker_size;
    Eigen::Vector3f color_list[2];
    color_list[0] = primary_rgb;
    color_list[1] = secondary_rgb;

    width = w;
    height = h;
    // data = new float[width * height * 3];
    data.resize(1);
    data[0].resize(width * height * 3);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Eigen::Vector3f color = color_list[((int)((x+1) / checker_size) + (int)((y+1) / checker_size)) % 2];
            data[0][(y * w + x) * 3] = color[0];
            data[0][(y * w + x) * 3 + 1] = color[1];
            data[0][(y * w + x) * 3 + 2] = color[2];
        }
    }

    return;
}


void Texture::genMipMap()
{
    std::cout << "Mipmap required" << std::endl;
    unsigned int layer_counter = 0;
    unsigned int new_w = width;
    unsigned int new_h = height;
    unsigned int w = width;
    unsigned int h = height;

    
    while(new_h > 1 && new_w > 1)
    {
        new_w /= 2;
        new_h /= 2;
        std::vector<float> new_layer;
        for (int i = 0; i < new_h; i++)
        {
            for (int j = 0; j < new_w; j++)
            {
                float r = 0, g = 0, b = 0;
                r += data[layer_counter][((2 * i) * new_w * 2 + 2*j) * 3];
                r += data[layer_counter][((2 * i) * new_w * 2 + 2*j + 1) * 3];
                r += data[layer_counter][((2*i+1) * new_w * 2 + 2*j) * 3];
                r += data[layer_counter][((2*i+1) * new_w * 2 + 2*j + 1) * 3];
                r /= 4;
                g += data[layer_counter][((2 * i) * new_w * 2 + 2*j) * 3 + 1];
                g += data[layer_counter][((2 * i) * new_w * 2 + 2*j + 1) * 3 + 1];
                g += data[layer_counter][((2*i+1) * new_w * 2 + 2*j) * 3 + 1];
                g += data[layer_counter][((2*i+1) * new_w * 2 + 2*j + 1) * 3 + 1];
                g /= 4;
                // g /= layer_counter;
                b += data[layer_counter][((2 * i) * new_w * 2 + 2*j) * 3 + 2];
                b += data[layer_counter][((2 * i) * new_w * 2 + 2*j + 1) * 3 + 2];
                b += data[layer_counter][((2*i+1) * new_w * 2 + 2*j) * 3 + 2];
                b += data[layer_counter][((2*i+1) * new_w * 2 + 2*j + 1) * 3 + 2];
                b /= 4;
                new_layer.push_back(r);
                new_layer.push_back(g);
                new_layer.push_back(b);
            }
        }

        data.push_back(new_layer);
        layer_counter++;
    }

    std::cout << "Generating mipmap done, " << data.size() << " layers generated." << std::endl;
    return;
}
