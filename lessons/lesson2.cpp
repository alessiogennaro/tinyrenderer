
#include "lesson2.h"

#include <array>
#include <cassert>
#include <utility>

#include "../colors.h"
#include "../geometry.h"
#include "../line.h"
#include "../tgaimage.h"


void triangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAImage& image, const TGAColor& color) {
    line(t0.x, t0.y, t1.x, t1.y, image, color);
    line(t1.x, t1.y, t2.x, t2.y, image, color);
    line(t2.x, t2.y, t0.x, t0.y, image, color); 
}

void filled_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, const TGAColor& color) {
    if (t0.y > t1.y) std::swap(t0, t1);
    // t1.y > t0.y
    if (t0.y > t2.y) std::swap(t0, t2);
    // t1.y >? t2.y > t0.y
    if (t1.y > t2.y) std::swap(t1, t2);
    
    // t2.y > t1.y > t0.y

    assert(t2.y > t1.y && t1.y > t0.y && t2.y > t0.y);

    // line(t0.x, t0.y, t1.x, t1.y, image, green); // boundary B
    // line(t1.x, t1.y, t2.x, t2.y, image, green); // boundary B
    // line(t2.x, t2.y, t0.x, t0.y, image, red);   // boundary A: between the highest and lowest point

    int total_height = t2.y - t0.y;

    for (int y = t0.y; y <= t1.y; y++) {
        int segment_height = t1.y - t0.y + 1;
        
        if (segment_height != 0. && total_height != 0.) {
            float alpha = (float) (y - t0.y) / total_height;
            float beta  = (float) (y - t0.y) / segment_height;

            Vec2i A = t0 + (t2 - t0) * alpha;
            Vec2i B = t0 + (t1 - t0) * beta;
            
            if (A.x > B.x) std::swap(A, B);

            for (int j = A.x; j <= B.x; j++)
                image.set(j, y, color);
        }
    }

    for (int y = t1.y; y <= t2.y; y++) {
        int segment_height = t2.y - t1.y + 1;
        
        if (segment_height != 0. && total_height != 0.) {
            float alpha = (float) (y - t0.y) / total_height;
            float beta  = (float) (y - t1.y) / segment_height;

            Vec2i A = t0 + (t2 - t0) * alpha;
            Vec2i B = t1 + (t2 - t1) * beta;

            if (A.x > B.x) std::swap(A, B);

            for (int j = A.x; j <= B.x; j++)
                image.set(j, y, color);
        }
    }
}

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
    if (std::abs(u.z) < 1) return Vec3f{-1, 1, 1};
    return Vec3f{1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

void filled_triangle(const std::array<Vec2i, 3>& pts, TGAImage& image, const TGAColor& color) {
    Vec2i bbox_min(image.get_width() - 1, image.get_height() - 1);
    Vec2i bbox_max(0, 0);
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

    for (std::size_t i = 0; i < pts.size(); i++) {
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

