
#include <array>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "colors.h"
#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle_renderer.h"
#include "triangle.h"
#include "wireframe_renderer.h"


#define RAND_COLOR   rand() % 255

float MIN_FLOAT = std::numeric_limits<float>::min();
float MAX_FLOAT = std::numeric_limits<float>::max();
std::unique_ptr<Model> model {};
Vec3f light_vec(0, 0, -1);



void rasterize(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color, int y_buffer[]) {
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float) (x1 - x0);
		int y = y0 * (1. - t) + y1 * t;

		// update y-buffer: always choose the highest point
		if (y_buffer[x] < y) {
			y_buffer[x] = y;
			image.set(x, 0, color);
		}
	}
	
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



int main(int argc, char** argv) {
	
	if (argc == 2) {
		model = std::make_unique<Model>(argv[1]);
	} else {
		model = std::make_unique<Model>("obj/african_head.obj");
	}

	TGAImage canvas(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

	/* -------------------------------------------------------------- */

	/*
	TGAImage render(CANVAS_WIDTH, 16, TGAImage::RGB);
	int y_buffer[CANVAS_WIDTH];

	for (int i = 0; i < CANVAS_WIDTH; i++) {
		y_buffer[i] = std::numeric_limits<int>::min();
	}

	rasterize( 20,  34, 744, 400, render, red,   y_buffer);
	rasterize(120, 434, 444, 400, render, green, y_buffer);
	rasterize(330, 463, 594, 200, render, blue,  y_buffer);
	line(10, 10, 790, 10, canvas, white);
	*/

	float* z_buffer = new float[CANVAS_WIDTH * CANVAS_HEIGHT];
    for (int i = CANVAS_WIDTH * CANVAS_HEIGHT; i >= 0; i--) {
		z_buffer[i] = -MAX_FLOAT;
	}

	for (int i=0; i<model->nfaces(); i++) {
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
			triangle(pts, z_buffer, canvas, TGAColor(color, color, color, 255));
		}

		// triangle(pts, z_buffer, canvas, TGAColor(RAND_COLOR, RAND_COLOR, RAND_COLOR, 255));
    }

	delete[] z_buffer;

	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the canvas
	canvas.flip_vertically();
	canvas.write_tga_file("output.tga");
	
	return 0;
}
