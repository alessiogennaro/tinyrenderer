
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
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


std::unique_ptr<Model> model {};
std::unique_ptr<int[]> z_buffer {};
Vec3f light_vec(0, 0, -1);


int main(int argc, char** argv) {
    
    if (argc == 3) {
        model = std::make_unique<Model>(argv[1]);
    } else {
        model = std::make_unique<Model>("obj/african_head.obj");
    }

    TGAImage canvas(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

    /* -------------------------------------------------------------- */


    z_buffer = std::make_unique<int[]>(CANVAS_WIDTH * CANVAS_HEIGHT);
    
    for (int i {0}; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
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
                (v.z + 1.) * COLOR_DEPTH / 2.
            };
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
        n.normalize();

        float intensity = n * light_vec;

        if (intensity > 0.) {
            std::array<Vec2i, 3> uv;
            for (int k {0}; k < (int) uv.size(); k++) {
                uv[k] = model->uv(i, k);
            }
            triangle(screen_coords, uv, canvas, intensity, z_buffer.get(), model.get());
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


    /* -------------------------------------------------------------- */

    // i want to have the origin at the left bottom corner of the canvas
    canvas.flip_vertically();
    canvas.write_tga_file("output.tga");
    
    return 0;
}
