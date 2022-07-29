
#ifndef __PROJECTION_RENDERER_H__
#define __PROJECTION_RENDERER_H__

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"


Vec3f matrix2vec(const Matrix& m);

Matrix vec2matrix(const Vec3f& v);

Matrix viewport(int x, int y, int w, int h);

/**
 * Supports perspective projection and camera positioning 
 */
void projection_renderer(Model* model, TGAImage& image, Vec3f light_vec, Vec3f camera);

#endif // __PROJECTION_RENDERER_H__
