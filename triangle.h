
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <array>

#include "geometry.h"
#include "line.h"
#include "tgaimage.h"

/**
 * Returns true if the point lays outside of the triangle, false otherwise
 */
bool outside(const Vec3f& barycentric);

/**
 * Returns the barycentic coordinates of the triangle pts and the point P
 */
Vec3f barycentric(const std::array<Vec2i, 3>& pts, Vec2i P);

/**
 * Draws a triangle and then fills it with color
 */
void triangle(const std::array<Vec2i, 3>& pts, TGAImage& image, const TGAColor& color);

#endif // __TRIANGLE_H__
