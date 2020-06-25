#include "qtcount.h"

bool qtcount::prunable(Node * root, int tol) {
    return prunableRecurse(root, tol, root->avg);
}

bool qtcount::prunableRecurse(Node * subRoot, int & tol, RGBAPixel & pixel1) {
    if (subRoot->SW == NULL)
    {
        return (pow((pixel1.r - subRoot->avg.r),2) + pow((pixel1.g - subRoot->avg.g),2) + pow((pixel1.b - subRoot->avg.b),2) <= tol);
    }

    return  prunableRecurse(subRoot->NW, tol, pixel1) && 
            prunableRecurse(subRoot->NE, tol, pixel1) && 
            prunableRecurse(subRoot->SW, tol, pixel1) && 
            prunableRecurse(subRoot->SE, tol, pixel1);
}
