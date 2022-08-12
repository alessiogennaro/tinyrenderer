
#include <array>
#include <cassert>
#include <utility>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "math_constants.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"


bool outside(const Vec3f& barycentric) {
    if (barycentric.x < 0) return true;
    if (barycentric.y < 0) return true;
    if (barycentric.z < 0) return true;
    return false;
}

Vec3f barycentric(const std::array<Vec2i, 3>& pts, Vec2i P) {
    Vec3f u = Vec3f{
        pts[2][0] - pts[0][0],
        pts[1][0] - pts[0][0],
        pts[0][0] - P[0]
    } ^ Vec3f{
        pts[2][1] - pts[0][1],
        pts[1][1] - pts[0][1],
        pts[0][1] - P[1]
    };
    /*
     * pts and P both have integer value as coordinates
     * so |u.z| < 1 means |u.z| == 0
     * that means the triangle is degenerate
     * and so we return something with negative coordinates
     */
    if (std::abs(u.z) < 1) return Vec3f{-1, 1, 1};

    return Vec3f{1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

Vec3f barycentric(const std::array<Vec3f, 3>& pts, Vec3f P) {
    Vec3f s[2];

    for (int i = 2; i--;) {
        s[i][0] = pts[2][i] - pts[0][i];
        s[i][1] = pts[1][i] - pts[0][i];
        s[i][2] = pts[0][i] - P[i];
    }

    Vec3f u = s[0] ^ s[1];

    if (std::abs(u.z) > 1e-2) {
        return Vec3f{1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
    } else {
        return Vec3f{-1, 1, 1};
    }
}

void triangle(const std::array<Vec2i, 3>& pts, TGAImage& image, const TGAColor& color) {
    Vec2i bbox_min(image.get_width() - 1, image.get_height() - 1);
    Vec2i bbox_max(0, 0);
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < (int) pts.size(); i++) {
        bbox_min.x =
            std::max(0, std::min(bbox_min.x, pts.at(i).x));
        bbox_min.y =
            std::max(0, std::min(bbox_min.y, pts.at(i).y));

        bbox_max.x =
            std::min(clamp.x, std::max(bbox_max.x, pts.at(i).x));
        bbox_max.y =
            std::min(clamp.y, std::max(bbox_max.y, pts.at(i).y));
    }

    Vec2i P {};

    for (P.x = bbox_min.x; P.x <= bbox_max.x; P.x = P.x + 1) {
        for (P.y = bbox_min.y; P.y <= bbox_max.y; P.y = P.y + 1) {

            Vec3f barycentric_canvas = barycentric(pts, P);
            if (outside(barycentric_canvas))
                continue;
            image.set(P.x, P.y, color);
        }
    }
}

void triangle(std::array<Vec3i, 3> t, std::array<Vec2i, 3> uv, TGAImage& image, float intensity, int* z_buffer, Model *model) {

    Vec3i&  t0 =  t[0];  Vec3i&  t1 =  t[1];  Vec3i&  t2 =  t[2];
    Vec2i& uv0 = uv[0];  Vec2i& uv1 = uv[1];  Vec2i& uv2 = uv[2];

    if (t0.y == t1.y && t0.y == t2.y) return;

    if (t0.y > t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); }
    if (t0.y > t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); }
    if (t1.y > t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); }

    int total_height = t2.y - t0.y;

    for (int i = 0; i < total_height; i++) {

        bool second_half = (i > t1.y - t0.y || t1.y == t0.y);
        int segment_height = second_half?  t2.y - t1.y  :  t1.y - t0.y;

        float alpha = (float) i / total_height;
        float beta  = (float) (i - (second_half? t1.y - t0.y : 0)) / segment_height;

        Vec3i   A =  t0 + Vec3f(t2 - t0) * alpha;
        Vec3i   B = second_half?  t1 + Vec3f(t2 - t1) * beta :  t0 + Vec3f(t1 - t0) * beta;
        Vec2i uvA = uv0 +    (uv2 - uv0) * alpha;
        Vec2i uvB = second_half? uv1 +    (uv2 - uv1) * beta : uv0 +    (uv1 - uv0) * beta;

        if (A.x > B.x) {
            std::swap(A, B); std::swap(uvA, uvB);
        }

        for (int j = A.x; j <= B.x; j++) {
            float phi = (B.x == A.x)?  1.  :  (float) (j - A.x) / (float) (B.x - A.x);

            Vec3i   P = Vec3f(A) + Vec3f(B - A) * phi;
            Vec2i uvP =     uvA  +  (uvB - uvA) * phi;
            int idx = P.x + P.y * CANVAS_WIDTH;

            if (z_buffer[idx] < P.z) {
                z_buffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);

                image.set(P.x, P.y, TGAColor(color.bgra[2] * intensity,
                                             color.bgra[1] * intensity,
                                              color.bgra[0] * intensity)
                );
            }
        }

    }

}

void triangle(const std::array<Vec3f, 3>& pts, float* z_buffer, TGAImage& image, const TGAColor& color) {
    Vec2f bbox_min(MAX_FLOAT, MAX_FLOAT);
    Vec2f bbox_max(MIN_FLOAT, MIN_FLOAT);
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < (int) pts.size(); i++) {
        for (int j = 0; j < 2; j++) {
            bbox_min[j] = std::max(0.f,      std::min(bbox_min[j], pts[i][j]));
            bbox_max[j] = std::min(clamp[j], std::max(bbox_max[j], pts[i][j]));
        }
    }

    Vec3f P {};

    for (P.x = bbox_min.x; P.x <= bbox_max.x; P.x++) {
        for (P.y = bbox_min.y; P.y <= bbox_max.y; P.y++) {

            Vec3f barycentric_canvas = barycentric(pts, P);
            if (outside(barycentric_canvas)) continue;
            P.z = 0;
            for (std::size_t j = 0; j < 3; j++) {
                P.z = P.z + (pts[j][2] * barycentric_canvas[j]);
            }
            if (z_buffer[ int(P.x + P.y * CANVAS_WIDTH) ] < P.z) {
                z_buffer[ int(P.x + P.y * CANVAS_WIDTH) ] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

Vec3f world2screen(Vec3f v) {
    return Vec3f{
        int((v.x + 1.) * CANVAS_WIDTH / 2. + .5),
        int((v.y + 1.) * CANVAS_HEIGHT / 2. + .5),
        v.z
    };
}
