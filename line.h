
#ifndef __LINE_H__
#define __LINE_H__

#include "tgaimage.h"

/**
 * Bresenham algorithm. Draws a line between (x0, y0) and (x1, y1).
 * TODO: add boundary checks to final version of line fun
 */
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

#endif // __LINE_H__
