
#ifndef __LINE_H__
#define __LINE_H__

#include "geometry.h"
#include "tgaimage.h"

/**
 * Bresenham algorithm. Draws a line between (x0, y0) and (x1, y1).
 * TODO: add boundary checks, we don't want to draw outside of the canvas
 */
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

/**
 * Bresenham algorithm. Draws a line between t0 and t1.
 */
void line(const Vec2i& t0, const Vec2i& t1, TGAImage& image, const TGAColor& color);

#endif // __LINE_H__
