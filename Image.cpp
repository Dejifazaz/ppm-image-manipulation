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

bool Image::loadFromFile(const std::string& filename)
{
    return false;
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
