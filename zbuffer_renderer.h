
#ifndef __ZBUFFER_RENDERER_H__
#define __ZBUFFER_RENDERER_H__

#include "geometry.h"
#include "model.h"
#include "tgaimage.h"


/**
 * Implements a zbuffer
 * 
 */
void zbuffer_renderer(Model const* model, TGAImage& image, Vec3f light_vec);

#endif // __ZBUFFER_RENDERER_H__
