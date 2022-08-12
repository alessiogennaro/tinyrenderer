
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "math_constants.h"
#include "model.h"
#include "projection_renderer.h"
#include "tgaimage.h"
#include "triangle.h"


std::unique_ptr<Model> model {};

// Vec3f light_vec(0, 0, -1);

Vec3f light_vec = Vec3f(1, -1, 1).normalize();
Vec3f camera(0, 0, 3);
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);



Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = (eye - center).normalize();       // z'
    Vec3f x = (up ^ z).normalize();             // x'
    Vec3f y = (z ^ x).normalize();              // y'

    Matrix lookat = Matrix::identity(4);

    for (int i = 0; i < 3; i++) {
        lookat[0][i] = x[i];
        lookat[1][i] = y[i];
        lookat[2][i] = z[i];

        lookat[i][3] = -center[i];
    }

    return lookat;
}

void triangle(std::array<Vec3i, 3> t, std::array<float, 3> ity, TGAImage& image, int* z_buffer) {

    Vec3i&   t0 =   t[0];  Vec3i&   t1 =   t[1];  Vec3i&  t2 =    t[2];
    float& ity0 = ity[0];  float& ity1 = ity[1];  float& ity2 = ity[2];

    if (t0.y == t1.y && t0.y == t2.y) return;

    if (t0.y > t1.y) { std::swap(t0, t1); std::swap(ity0, ity1); }
    if (t0.y > t2.y) { std::swap(t0, t2); std::swap(ity0, ity2); }
    if (t1.y > t2.y) { std::swap(t1, t2); std::swap(ity1, ity2); }

    int total_height = t2.y - t0.y;

    for (int i = 0; i < total_height; i++) {

        bool second_half = (i > t1.y - t0.y || t1.y == t0.y);
        int segment_height = second_half?  t2.y - t1.y  :  t1.y - t0.y;

        float alpha = (float) i / total_height;
        float beta  = (float) (i - (second_half? t1.y - t0.y : 0)) / segment_height;

        Vec3i    A =  t0 + Vec3f(t2 - t0) * alpha;
        Vec3i    B = second_half?   t1 + Vec3f(t2 - t1) * beta :   t0 + Vec3f(t1 - t0) * beta;
        float ityA = ity0 + (ity2 - ity0) * alpha;
        float ityB = second_half? ity1 +  (ity2 - ity1) * beta : ity0 + (ity1 - ity0) * beta;

        if (A.x > B.x) {
            std::swap(A, B); std::swap(ityA, ityB);
        }

        for (int j = A.x; j <= B.x; j++) {
            float phi = (B.x == A.x)?  1.  :  (float) (j - A.x) / (B.x - A.x);

            Vec3i    P = Vec3f(A) + Vec3f(B - A) * phi;
            float ityP =    ityA  +  (ityB - ityA) * phi;
            int idx = P.x + (P.y * CANVAS_WIDTH);

            if (P.x >= CANVAS_WIDTH  || P.y >= CANVAS_HEIGHT || P.x <= 0 || P.y <= 0)
                continue;

            if (z_buffer[idx] < P.z) {
                z_buffer[idx] = P.z;
                image.set(P.x, P.y, TGAColor(255, 255, 255) * ityP);
            }
        }

    }

}



int main(int argc, char** argv) {
    
    if (argc == 3) {
        model = std::make_unique<Model>(argv[1]);
    } else {
        model = std::make_unique<Model>("obj/african_head.obj");
    }

    TGAImage canvas(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

    /* -------------------------------------------------------------- */


    auto z_buffer = std::make_unique<int[]>(CANVAS_WIDTH * CANVAS_HEIGHT);
    for (int i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
        z_buffer[i] = MIN_INT;
    }


    Matrix ModelView  = lookat(eye, center, Vec3f(0, 1, 0));
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(
        CANVAS_WIDTH / 8,
        CANVAS_HEIGHT / 8,
        CANVAS_WIDTH * 3/4,
        CANVAS_HEIGHT * 3/4
    );
    Projection[3][2] = -1.f / (eye - center).norm();

    Matrix Z = (ViewPort * Projection * ModelView);

#ifdef DEBUG
        std::cerr << ModelView << '\n';
        std::cerr << Projection << '\n';
        std::cerr << ViewPort << '\n';
        std::cerr << Z << '\n';
#endif


    for (int i = 0; i < model->nfaces(); i++) {

        std::vector<int> face = model->face(i);
        std::array<Vec3i, 3> screen_coords {};
        std::array<Vec3f, 3> world_coords  {};
        std::array<float, 3> intensity     {};

        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);

            screen_coords[j] = Vec3f{
                ViewPort * Projection * ModelView * Matrix(v)
            };
            world_coords[j] = v;
            intensity[j] = model->norm(i, j) * light_vec;
        }
        
        triangle(screen_coords, intensity, canvas, z_buffer.get());

    }

    TGAImage zbimage(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::GRAYSCALE);
    
    for (int i = 0; i < CANVAS_WIDTH; i++) {
        for (int j = 0; j < CANVAS_HEIGHT; j++) {
            zbimage.set(i, j, TGAColor(z_buffer[i + j * CANVAS_WIDTH]));
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
