
#include <array>
#include <memory>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "math_constants.h"
#include "model.h"
#include "projection_renderer.h"
#include "tgaimage.h"
#include "triangle.h"


Vec3f matrix2vec(const Matrix& m) {
    const float& z = m[3][0];
    const float& u = m[0][0], v = m[1][0], w = m[2][0];

    return Vec3f{u / z, v / z, w / z};
}

Matrix vec2matrix(const Vec3f& v) {
    Matrix m(4, 1);

    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;

    return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);

    m[0][3] = x + (w / 2.f);
    m[1][3] = y + (h / 2.f);
    m[2][3] = COLOR_DEPTH / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = COLOR_DEPTH / 2.f;

    return m;
}

void projection_renderer(Model* model, TGAImage& image, Vec3f light_vec, Vec3f camera) {
    auto z_buffer = std::make_unique<int[]>(CANVAS_WIDTH * CANVAS_HEIGHT);
    for (int i {0}; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
        z_buffer[i] = MIN_INT;
    }

    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(
        CANVAS_WIDTH / 8,
        CANVAS_HEIGHT / 8,
        CANVAS_WIDTH * 3/4,
        CANVAS_HEIGHT * 3/4
    );
    Projection[3][2] = -1.f / camera.z;

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face {model->face(i)};
        std::array<Vec3i, 3> screen_coords {};
        std::array<Vec3f, 3> world_coords  {};

        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert( face[j] );

            screen_coords[j] = matrix2vec(ViewPort * Projection * vec2matrix(v));
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
