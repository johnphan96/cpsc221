#include "gridColorPicker.h"

gridColorPicker::gridColorPicker(HSLAPixel gridColor, int gridSpacing)
{
    color = gridColor;
    spacing = gridSpacing;
}

HSLAPixel gridColorPicker::operator()(int x, int y)
{
    HSLAPixel ret;
    if ((y - x) % spacing == 0 || (y + x) % spacing == 0)
    {
        ret = color;
    }
    else
    {
        ret = HSLAPixel(0, 0, 1, 0);
    }

    return ret;
}
