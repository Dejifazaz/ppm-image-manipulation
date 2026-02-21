#include "Image.h"
#include <fstream>

Image::Image()
{
    width = 0;
    height = 0;
    positionX = 0;
    positionY = 0;
}

Image::~Image()
{
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

void Image::setSize(int w, int h)
{
    width = w;
    height = h;
    pixels.resize(w * h);
}

int Image::getPositionX() const
{
    return positionX;
}

int Image::getPositionY() const
{
    return positionY;
}

void Image::setPosition(int x, int y)
{
    positionX = x;
    positionY = y;
}

std::vector<RGB>& Image::getPixels()
{
    return pixels;
}

const std::vector<RGB>& Image::getPixels() const
{
    return pixels;
}

RGB Image::getPixel(int x, int y) const
{
    if(x < 0 || x >= width || y < 0 || y >= height)
    {
        RGB black;
        black.r = 0;
        black.g = 0;
        black.b = 0;
        return black;
    }
    return pixels[y * width + x];
}

void Image::setPixel(int x, int y, RGB colour)
{
    if(x >= 0 && x < width && y >= 0 && y < height)
    {
        pixels[y * width + x] = colour;
    }
}

/*
    Load a P6 ppm file. One byte per channel only.
    Returns false if file missing, not P6, or invalid.
*/
bool Image::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if(!file)
    {
        return false;
    }

    std::string magic;
    file >> magic;
    if(magic != "P6")
    {
        return false;
    }

    int w, h, maxval;
    file >> w >> h >> maxval;
    if(w <= 0 || h <= 0 || maxval <= 0 || maxval > 255)
    {
        return false;
    }

    file.get();
    width = w;
    height = h;
    pixels.resize(width * height);

    for(int i = 0; i < width * height; i++)
    {
        unsigned char r, g, b;
        file.get((char&)r);
        file.get((char&)g);
        file.get((char&)b);
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
    }

    file.close();
    return true;
}

bool Image::saveToFile(const std::string& filename) const
{
    return false;
}

void Image::filterRed() {}
void Image::filterGreen() {}
void Image::filterBlue() {}
void Image::greyscale() {}
void Image::flipHorizontal() {}
void Image::flipVertical() {}
void Image::rotate90() {}
void Image::gaussianBlur(int radius) {}
void Image::crop(int x, int y, int cropWidth, int cropHeight) {}
void Image::mirrorImage() {}
void Image::to4Bit() {}
void Image::invert() {}
void Image::adjustBrightness(int delta) {}
void Image::sepia() {}
void Image::sharpen() {}
