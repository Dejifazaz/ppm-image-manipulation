#include "Image.h"
#include <fstream>
#include <cmath>

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

/*
    Save the image as a P6 ppm file.
*/
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
/*
    Convert image to greyscale using standard weights.
*/
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
/*
    Rotate image 90 degrees clockwise.
*/
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
/*
    Gaussian blur. Radius 2 is a good default.
*/
void Image::gaussianBlur(int radius)
{
    if(radius <= 0)
        return;

    int size = 2 * radius + 1;
    std::vector<double> kernel(size * size);
    double sigma = radius / 2.0;
    double sum = 0;

    for(int dy = -radius; dy <= radius; dy++)
    {
        for(int dx = -radius; dx <= radius; dx++)
        {
            double v = exp(-(dx*dx + dy*dy) / (2 * sigma * sigma));
            kernel[(dy + radius) * size + (dx + radius)] = v;
            sum = sum + v;
        }
    }
    for(int i = 0; i < size * size; i++)
    {
        kernel[i] = kernel[i] / sum;
    }

    std::vector<RGB> result(width * height);
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            double r = 0, g = 0, b = 0;
            for(int dy = -radius; dy <= radius; dy++)
            {
                for(int dx = -radius; dx <= radius; dx++)
                {
                    RGB p = getPixel(x + dx, y + dy);
                    double w = kernel[(dy + radius) * size + (dx + radius)];
                    r = r + p.r * w;
                    g = g + p.g * w;
                    b = b + p.b * w;
                }
            }
            if(r < 0) r = 0;
            if(r > 255) r = 255;
            if(g < 0) g = 0;
            if(g > 255) g = 255;
            if(b < 0) b = 0;
            if(b > 255) b = 255;
            result[y * width + x].r = (unsigned char)r;
            result[y * width + x].g = (unsigned char)g;
            result[y * width + x].b = (unsigned char)b;
        }
    }
    pixels = result;
}

void Image::crop(int x, int y, int cropWidth, int cropHeight)
{
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x + cropWidth > width)
        cropWidth = width - x;
    if(y + cropHeight > height)
        cropHeight = height - y;
    if(cropWidth <= 0 || cropHeight <= 0)
        return;

    std::vector<RGB> cropped(cropWidth * cropHeight);
    for(int cy = 0; cy < cropHeight; cy++)
    {
        for(int cx = 0; cx < cropWidth; cx++)
        {
            cropped[cy * cropWidth + cx] = getPixel(x + cx, y + cy);
        }
    }
    width = cropWidth;
    height = cropHeight;
    pixels = cropped;
}
/*
    Mirror: left half stays, right half is reflection of left.
*/
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
/*
    Reduce to 4-bit colour (16 levels per channel).
*/
void Image::to4Bit()
{
    for(int i = 0; i < width * height; i++)
    {
        int v;
        v = (pixels[i].r >> 4) & 15;
        pixels[i].r = (unsigned char)(v * 255 / 15);
        v = (pixels[i].g >> 4) & 15;
        pixels[i].g = (unsigned char)(v * 255 / 15);
        v = (pixels[i].b >> 4) & 15;
        pixels[i].b = (unsigned char)(v * 255 / 15);
    }
}
void Image::invert()
{
    for(int i = 0; i < width * height; i++)
    {
        pixels[i].r = 255 - pixels[i].r;
        pixels[i].g = 255 - pixels[i].g;
        pixels[i].b = 255 - pixels[i].b;
    }
}

void Image::adjustBrightness(int delta)
{
    for(int i = 0; i < width * height; i++)
    {
        int r = pixels[i].r + delta;
        int g = pixels[i].g + delta;
        int b = pixels[i].b + delta;
        if(r < 0) r = 0;
        if(r > 255) r = 255;
        if(g < 0) g = 0;
        if(g > 255) g = 255;
        if(b < 0) b = 0;
        if(b > 255) b = 255;
        pixels[i].r = (unsigned char)r;
        pixels[i].g = (unsigned char)g;
        pixels[i].b = (unsigned char)b;
    }
}

void Image::sepia()
{
    for(int i = 0; i < width * height; i++)
    {
        int r = (int)(0.393 * pixels[i].r + 0.769 * pixels[i].g + 0.189 * pixels[i].b);
        int g = (int)(0.349 * pixels[i].r + 0.686 * pixels[i].g + 0.168 * pixels[i].b);
        int b = (int)(0.272 * pixels[i].r + 0.534 * pixels[i].g + 0.131 * pixels[i].b);
        if(r > 255) r = 255;
        if(g > 255) g = 255;
        if(b > 255) b = 255;
        pixels[i].r = (unsigned char)r;
        pixels[i].g = (unsigned char)g;
        pixels[i].b = (unsigned char)b;
    }
}

/*
    Sharpen using a simple 3x3 kernel.
*/
void Image::sharpen()
{
    int kernel[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
    std::vector<RGB> result(width * height);
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            int r = 0, g = 0, b = 0;
            for(int dy = -1; dy <= 1; dy++)
            {
                for(int dx = -1; dx <= 1; dx++)
                {
                    RGB p = getPixel(x + dx, y + dy);
                    int k = kernel[(dy + 1) * 3 + (dx + 1)];
                    r = r + p.r * k;
                    g = g + p.g * k;
                    b = b + p.b * k;
                }
            }
            if(r < 0) r = 0;
            if(r > 255) r = 255;
            if(g < 0) g = 0;
            if(g > 255) g = 255;
            if(b < 0) b = 0;
            if(b > 255) b = 255;
            result[y * width + x].r = (unsigned char)r;
            result[y * width + x].g = (unsigned char)g;
            result[y * width + x].b = (unsigned char)b;
        }
    }
    pixels = result;
}
