
#include "stats.h"
#include <cmath>
#include <vector>
#include <numeric>
#include <stdio.h>

stats::stats(PNG &im)
{
    sumRed.resize(im.width(), vector<long>(im.height(), 0));
    sumBlue.resize(im.width(), vector<long>(im.height(), 0));
    sumGreen.resize(im.width(), vector<long>(im.height(), 0));

    sumsqRed.resize(im.width(), vector<long>(im.height(), 0));
    sumsqBlue.resize(im.width(), vector<long>(im.height(), 0));
    sumsqGreen.resize(im.width(), vector<long>(im.height(), 0));

    for (unsigned int x = 0; x < im.width(); x++)
    {
        for (unsigned int y = 0; y < im.height(); y++)
        {
            RGBAPixel *pixel = im.getPixel(x, y);
            if (y == 0 && x == 0)
            {
                sumRed[x][y] = (pixel->r);
                sumGreen[x][y] = (pixel->g);
                sumBlue[x][y] = (pixel->b);

                sumsqRed[x][y] = pow((pixel->r), 2);
                sumsqGreen[x][y] = pow((pixel->g), 2);
                sumsqBlue[x][y] = pow((pixel->b), 2);
                continue;
            }
            if (y < 1) {
                sumRed[x][y] = (sumRed[x - 1][y] + pixel->r);
                sumGreen[x][y] = (sumGreen[x - 1][y] + pixel->g);
                sumBlue[x][y] = (sumBlue[x - 1][y] + pixel->b);

                sumsqRed[x][y] = sumsqRed[x - 1][y] + pow((pixel->r), 2);
                sumsqGreen[x][y] = sumsqGreen[x - 1][y] + pow((pixel->g), 2);
                sumsqBlue[x][y] = sumsqBlue[x - 1][y] + pow((pixel->b), 2);
                continue;
            }

            if (x < 1) {
                sumRed[x][y] = sumRed[x][y - 1] + pixel->r;
                sumGreen[x][y] = sumGreen[x][y - 1] + pixel->g;
                sumBlue[x][y] = sumBlue[x][y - 1] + pixel->b;

                sumsqRed[x][y] = sumsqRed[x][y - 1] + pow((pixel->r), 2);
                sumsqGreen[x][y] = sumsqGreen[x][y - 1] + pow((pixel->g), 2);
                sumsqBlue[x][y] = sumsqBlue[x][y - 1] + pow((pixel->b), 2);
                continue;
            } 
            if (x >= 1 && y >= 1)
            {
                sumRed[x][y] = pixel->r + sumRed[x][y - 1] + sumRed[x - 1][y] - sumRed[x - 1][y - 1];
                sumGreen[x][y] = pixel->g + sumGreen[x][y - 1] + sumGreen[x - 1][y] - sumGreen[x - 1][y - 1];
                sumBlue[x][y] = pixel->b + sumBlue[x][y - 1] + sumBlue[x - 1][y] - sumBlue[x - 1][y - 1];

                sumsqRed[x][y] = pow((pixel->r), 2) + sumsqRed[x][y - 1] + sumsqRed[x - 1][y] - sumsqRed[x - 1][y - 1];
                sumsqGreen[x][y] = pow((pixel->g), 2) + sumsqGreen[x][y - 1] + sumsqGreen[x - 1][y] - sumsqGreen[x - 1][y - 1];
                sumsqBlue[x][y] = pow((pixel->b), 2) + sumsqBlue[x][y - 1] + sumsqBlue[x - 1][y] - sumsqBlue[x - 1][y - 1];
                continue;
            }
        }
    }
}

long stats::getSum(char channel, pair<int,int> ul, int dim) {
    long sum = 0;
    long topBound = 0;
    long leftBound = 0;
    long topLeft = 0;
    int y = ul.second + pow(2, dim) - 1;
    int x = ul.first + pow(2, dim) - 1;

    if (channel == 'r') {

        if (ul.first > 0) {
            leftBound = sumRed[x - pow(2, dim)][y];
        }
        
        if (ul.second > 0) {
            topBound = sumRed[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0)
        {
            topLeft = sumRed[ul.first - 1][ul.second - 1];
        }
        sum = sumRed[x][y] - leftBound - topBound + topLeft;
    } 
    else if (channel == 'g') {
        if (ul.first > 0) {
            leftBound = sumGreen[x - pow(2, dim)][y];
        }
        
        if (ul.second > 0) {
            topBound = sumGreen[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0)
        {
            topLeft = sumGreen[ul.first - 1][ul.second - 1];
        }
        sum = sumGreen[x][y] - leftBound - topBound + topLeft;
    } 
    else {
        if (ul.first > 0)
        {
            leftBound = sumBlue[x - pow(2, dim)][y];
        }

        if (ul.second > 0)
        {
            topBound = sumBlue[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0)
        {
            topLeft = sumBlue[ul.first - 1][ul.second - 1];
        }
        sum = sumBlue[x][y] - leftBound - topBound + topLeft;
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int dim){
    long sum = 0;
    long topBound = 0;
    long leftBound = 0;
    long topLeft = 0;
    int y = ul.second + pow(2, dim) - 1;
    int x = ul.first + pow(2, dim) - 1;

    if (channel == 'r')
    {

        if (ul.first > 0)
        {
            leftBound = sumsqRed[x - pow(2, dim)][y];
        }

        if (ul.second > 0)
        {
            topBound = sumsqRed[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0 )
        {
            topLeft = sumsqRed[ul.first - 1][ul.second - 1];
        }
        sum = sumsqRed[x][y] - leftBound - topBound + topLeft;
    }
    else if (channel == 'g')
    {
        if (ul.first > 0)
        {
            leftBound = sumsqGreen[x - pow(2, dim)][y];
        }

        if (ul.second > 0)
        {
            topBound = sumsqGreen[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0 )
        {
            topLeft = sumsqGreen[ul.first - 1][ul.second - 1];
        }
        sum = sumsqGreen[x][y] - leftBound - topBound + topLeft;
    }
    else
    {
        if (ul.first > 0)
        {
            leftBound = sumsqBlue[x - pow(2, dim)][y];
        }

        if (ul.second > 0)
        {
            topBound = sumsqBlue[x][y - pow(2, dim)];
        }

        if (ul.first > 0 && ul.second > 0 )
        {
            topLeft = sumsqBlue[ul.first - 1][ul.second - 1];
        }
        sum = sumsqBlue[x][y] - leftBound - topBound + topLeft;
    }
    return sum;
}

long stats::rectArea(int dim) {
    return (long) pow(4, dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int dim){
    long sumSquaredRed = getSumSq('r', ul, dim);
    long sumSquaredGreen = getSumSq('g', ul, dim);
    long sumSquaredBlue = getSumSq('b', ul, dim);
    long sumRed = getSum('r', ul, dim);
    long sumGreen = getSum('g', ul, dim);
    long sumBlue = getSum('b', ul, dim);
    long area = rectArea(dim);
    double var = (double) ((sumSquaredRed + sumSquaredGreen + sumSquaredBlue) - (pow(sumRed,2)/area + pow(sumGreen, 2)/area + pow(sumBlue, 2)/area));

    return var;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int dim){
    long area = rectArea(dim);
    long r = (int) getSum('r', ul, dim) / area;
    long g = (int) getSum('g', ul, dim) / area;
    long b = (int) getSum('b', ul, dim) / area;

    RGBAPixel pixel(r, g, b);

    return pixel;
}
