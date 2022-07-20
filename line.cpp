
#include <cmath>
#include <iostream>
#include <utility>

#include "line.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {

	bool steep = false;

	int width  = std::abs(x0 - x1);
	int height = std::abs(y0 - y1);

	if (height > width) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror = std::abs(dy) * 2;
	int error = 0;
	int y = y0;
	const int yincr = (y1 > y0? 1 : -1);

	for (int x = x0; x <= x1; x++) {
		
		if (steep) {
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}

		error = error + derror;
		
		if (error > dx) {
			y = y + yincr;
			error = error - (dx * 2);
		}
	}

}

void line(const Vec2i& t0, const Vec2i& t1, TGAImage& image, const TGAColor& color) {
	line(t0.x, t0.y, t1.x, t1.y, image, color);
}

