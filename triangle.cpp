
#include <array>
#include <cassert>
#include <utility>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "math_constants.h"
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

void triangle(const std::array<Vec3f, 3>& pts, float* z_buffer, TGAImage& image, const TGAColor& color) {
	Vec2f bbox_min(MAX_FLOAT, MAX_FLOAT);
	Vec2f bbox_max(MIN_FLOAT, MIN_FLOAT);
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

	for (std::size_t i = 0; i < pts.size(); i++) {
		for (std::size_t j = 0; j < 2; j++) {
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
