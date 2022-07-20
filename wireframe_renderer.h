
#ifndef __WIREFRAME_RENDERER_H__
#define __WIREFRAME_RENDERER_H__

#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"

void wireframe_renderer(Model const* model, TGAImage& image);

#endif // __WIREFRAME_RENDERER_H__
