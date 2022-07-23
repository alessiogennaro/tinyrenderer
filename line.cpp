
#include <cmath>
#include <iostream>
#include <utility>

#include "constants.h"
#include "line.h"
#include "tgaimage.h"
#include "tinyrenderer_exception.h"


void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {

	if (x0 < 0 || x0 > CANVAS_WIDTH - 1 || x1 < 0 || x1 > CANVAS_WIDTH - 1
	 || y0 < 0 || y0 > CANVAS_HEIGHT - 1 || y1 < 0 || y1 > CANVAS_HEIGHT - 1) {
		throw tinyrenderer_exception("Can't draw a line outside of canvas!");
	}

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
