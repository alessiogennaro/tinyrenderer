
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
#include "model.h"
#include "tgaimage.h"
#include "triangle_renderer.h"
#include "triangle.h"
#include "wireframe_renderer.h"


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


std::unique_ptr<Model> model {};


int main(int argc, char** argv) {
	
	if (argc == 2) {
		model = std::make_unique<Model>(argv[1]);
	} else {
		model = std::make_unique<Model>("obj/african_head.obj");
	}

	// TGAImage canvas(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

	/* -------------------------------------------------------------- */

	TGAImage render(CANVAS_WIDTH, 16, TGAImage::RGB);
	int y_buffer[CANVAS_WIDTH];

	for (int i = 0; i < CANVAS_WIDTH; i++) {
		y_buffer[i] = std::numeric_limits<int>::min();
	}

	rasterize( 20,  34, 744, 400, render, red,   y_buffer);
	rasterize(120, 434, 444, 400, render, green, y_buffer);
	rasterize(330, 463, 594, 200, render, blue,  y_buffer);

	// line(10, 10, 790, 10, canvas, white);

	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the canvas
	render.flip_vertically();
	render.write_tga_file("output.tga");
	
	return 0;
}
