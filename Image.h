#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

/*
    RGB pixel - one byte per colour (0 to 255).
*/
struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

/*
    Image class for PPM (P6) images.
    Pixels stored in order: top-left to bottom-right, row by row.
    Index = (y * width) + x
*/
class Image
{
public:
    Image();
    ~Image();

    int getWidth() const;
    int getHeight() const;
    void setSize(int w, int h);

    int getPositionX() const;
    int getPositionY() const;
    void setPosition(int x, int y);

    std::vector<RGB>& getPixels();
    const std::vector<RGB>& getPixels() const;

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

    void filterRed();
    void filterGreen();
    void filterBlue();
    void greyscale();
    void flipHorizontal();
    void flipVertical();

    void rotate90();
    void gaussianBlur(int radius);
    void crop(int x, int y, int cropWidth, int cropHeight);
    void mirrorImage();
    void to4Bit();

    void invert();
    void adjustBrightness(int delta);
    void sepia();
    void sharpen();

private:
    int width;
    int height;
    int positionX;
    int positionY;
    std::vector<RGB> pixels;

    RGB getPixel(int x, int y) const;
    void setPixel(int x, int y, RGB colour);
};

#endif
