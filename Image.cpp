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
    std::ofstream file(filename.c_str(), std::ios::binary);
    if(!file)
    {
        return false;
    }

    file << "P6\n" << width << " " << height << "\n255\n";
    for(int i = 0; i < width * height; i++)
    {
        file.put(pixels[i].r);
        file.put(pixels[i].g);
        file.put(pixels[i].b);
    }

    file.close();
    return true;
}

void Image::filterRed()
{
    for(int i = 0; i < width * height; i++)
    {
        pixels[i].g = 0;
        pixels[i].b = 0;
    }
}

void Image::filterGreen()
{
    for(int i = 0; i < width * height; i++)
    {
        pixels[i].r = 0;
        pixels[i].b = 0;
    }
}

void Image::filterBlue()
{
    for(int i = 0; i < width * height; i++)
    {
        pixels[i].r = 0;
        pixels[i].g = 0;
    }
}
void Image::greyscale()
{
    for(int i = 0; i < width * height; i++)
    {
        double grey = 0.299 * pixels[i].r + 0.587 * pixels[i].g + 0.114 * pixels[i].b;
        if(grey > 255)
            grey = 255;
        unsigned char g = (unsigned char)grey;
        pixels[i].r = g;
        pixels[i].g = g;
        pixels[i].b = g;
    }
}
void Image::flipHorizontal()
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width / 2; x++)
        {
            int left = y * width + x;
            int right = y * width + (width - 1 - x);
            RGB temp = pixels[left];
            pixels[left] = pixels[right];
            pixels[right] = temp;
        }
    }
}

void Image::flipVertical()
{
    for(int y = 0; y < height / 2; y++)
    {
        for(int x = 0; x < width; x++)
        {
            int top = y * width + x;
            int bottom = (height - 1 - y) * width + x;
            RGB temp = pixels[top];
            pixels[top] = pixels[bottom];
            pixels[bottom] = temp;
        }
    }
}
void Image::rotate90()
{
    std::vector<RGB> rotated(width * height);
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            int newX = height - 1 - y;
            int newY = x;
            rotated[newY * height + newX] = getPixel(x, y);
        }
    }
    int temp = width;
    width = height;
    height = temp;
    pixels = rotated;
}
void Image::mirrorImage()
{
    int half = width / 2;
    for(int y = 0; y < height; y++)
    {
        for(int x = half; x < width; x++)
        {
            int srcX = width - 1 - x;
            pixels[y * width + x] = getPixel(srcX, y);
        }
    }
}
