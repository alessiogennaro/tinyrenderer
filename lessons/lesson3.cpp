
#include "lesson3.h"

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
