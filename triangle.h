
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "geometry.h"
#include "line.h"
#include "tgaimage.h"

/**
 * Draws a triangle using t0, t1 and t2 as vertices
 */
void triangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAImage& image, const TGAColor& color);

/**
 * Draws a triangle and then fills it with color
 */
void filled_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, const TGAColor& color);

#endif // __TRIANGLE_H__
