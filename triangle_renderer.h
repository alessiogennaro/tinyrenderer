
#ifndef __TRIANGLE_RENDERER_H__
#define __TRIANGLE_RENDERER_H__

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

/**
 * Implements back-face culling and Gouraud shading
 */
void triangle_renderer(Model const* model, TGAImage& image, Vec3f light_vec);

#endif // __TRIANGLE_RENDERER_H__
