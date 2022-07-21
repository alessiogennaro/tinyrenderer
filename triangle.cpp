
#include <array>
#include <cassert>
#include <utility>

#include "colors.h"
#include "geometry.h"
#include "line.h"
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
    if (std::abs(u.z) < 1) return Vec3f{-1, 1, 1};
    return Vec3f{1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z};
}

void triangle(const std::array<Vec2i, 3>& pts, TGAImage& image, const TGAColor& color) {
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
