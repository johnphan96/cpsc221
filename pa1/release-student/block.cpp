#include "block.h"
#include "cs221util/PNG.h"

/** 
* Creates a block that is width pixels wide.
* starting at (x, 0) on img.
*/
void Block::build(PNG &img, int x, int width) {

    int blockWidth = width + x;
    int blockHeight = img.height();
 
    for(int row = 0; row < blockHeight; row++) {
        vector<HSLAPixel> pixelsOfRow;
        for(int col = x; col < blockWidth; col++){
            HSLAPixel *pixel = img.getPixel(col, row);
            pixelsOfRow.push_back(*pixel);
            
        }
        data.push_back(pixelsOfRow);
    }
};

/** 
* Renders the given block onto img with its upper
* left corner at (x, 0)
*/
void Block::render(PNG &img, int x) const{
    int blockWidth = x + width();

    for (int row = 0; row < height(); row++){
        for (int col = x; col < blockWidth; col++){
            *(img.getPixel(col, row)) = data[row][col-x];
        }
    }
}

/**
* Creates a "pastel" version of the block by halving
* the saturation and doubling the luminesence of each pixel.
 */
void Block::pastel()
{
    for (int x = 0; x < width(); x++)
    {
        for (int y = 0; y < height(); y++)
        {
            data[y][x].s *= 0.5;
            if (data[y][x].l * 2 > 1.0) {
                data[y][x].l = 1.0;
            } else {
                data[y][x].l *= 2;
            }
            
        }
    }
}

// Return the width and height of the block.
int Block::width() const{
    return data[0].size();
}

int Block::height() const {
    return data.size();
}