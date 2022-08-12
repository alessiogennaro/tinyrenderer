
#include <array>
#include <memory>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "math_constants.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"
#include "zbuffer_renderer.h"


void zbuffer_renderer(Model* model, TGAImage& image, Vec3f light_vec) {
    
    auto z_buffer = std::make_unique<int[]>(CANVAS_WIDTH * CANVAS_HEIGHT);
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
        z_buffer[i] = MIN_INT;
    }

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face {model->face(i)};
        std::array<Vec3i, 3> screen_coords {};
        std::array<Vec3f, 3> world_coords  {};

        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert( face[j] );

            screen_coords[j] = Vec3i{
                (v.x + 1.) * CANVAS_WIDTH / 2.,
                (v.y + 1.) * CANVAS_HEIGHT / 2.,
                (v.z + 1.) * Z_BUFFER_DEPTH / 2.
            };
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
        n.normalize();

        float intensity = n * light_vec;

        if (intensity > 0.) {
            std::array<Vec2i, 3> uv;
            for (int k = 0; k < (int) uv.size(); k++) {
                uv[k] = model->uv(i, k);
            }
            triangle(screen_coords, uv, image, intensity, z_buffer.get(), model);
        }
    }

    TGAImage zbimage(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::GRAYSCALE);
    
    for (int i = 0; i < CANVAS_WIDTH; i++) {
        for (int j = 0; j < CANVAS_HEIGHT; j++) {
            zbimage.set(i, j, TGAColor(z_buffer[i + j * CANVAS_WIDTH], 1));
        }
    }
    zbimage.flip_vertically();
    zbimage.write_tga_file("zbuffer.tga");
}
