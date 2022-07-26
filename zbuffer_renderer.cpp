
#include "zbuffer_renderer.h"

#include <array>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "math_constants.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"

void zbuffer_renderer(Model const* model, TGAImage& image, Vec3f light_vec) {
    float* z_buffer = new float[CANVAS_WIDTH * CANVAS_HEIGHT];
    for (int i = CANVAS_WIDTH * CANVAS_HEIGHT; i >= 0; i--) {
        z_buffer[i] = -MAX_FLOAT;
    }

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        std::array<Vec3f, 3> pts {};
        std::array<Vec3f, 3> world_coords {};
        
        for (int j = 0; j < 3; j++) {
            pts[j] = world2screen(model->vert( face[j] ));
            world_coords[j] = model->vert( face[j] );
        }
        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
        n.normalize();

        float intensity = n * light_vec;

        if (intensity > 0.) {
            int color = intensity * 255;
            triangle(pts, z_buffer, image, TGAColor(color, color, color, 255));
        }

    }

    delete[] z_buffer;
}
