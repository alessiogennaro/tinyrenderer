
#include <cmath>
#include <memory>
#include <utility>

#include "lesson1.h"
#include "../tgaimage.h"

void line_1a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (float t = 0.; t < 1.; t = t + 0.01) { // Problem: why 0.01?
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;

		image.set(x, y, color);
	}
}

void line_1a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color, float step) {
	for (float t = 0.; t < 1.; t = t + step) {
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;

		image.set(x, y, color);
	}
}

void line_2a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float) (x1 - x0);
		int y = y0 * (1. - t) + y1 * t;

		image.set(x, y, color);
	}
}

void line_3a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	
	bool steep = false;

	int width  = std::abs(x0 - x1);
	int height = std::abs(y0 - y1);

	// height > width, the line is steep
	// the swaps make sure the line is no more steep
	if (height > width) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	// x0 should always preceed x1
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	// let's draw
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float) (x1 - x0); // Problem: how can we optimize this?
		int y = y0 * (1. - t) + y1 * t;

		// we replaced x and y coordinates
		if (steep) {
			// now it's fine
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}
	}

}

void line_4a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	
	bool steep = false;

	int width  = std::abs(x0 - x1);
	int height = std::abs(y0 - y1);

	// height > width, the line is steep
	// the swaps make sure the line is no more steep
	if (height > width) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	// x0 should always preceed x1
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0; // width
	int dy = y1 - y0; // height

	float derror = std::abs(dy / float(dx));
	
	// the error variable gives us the distance to the best straight line
	// from our current (x, y) pixel
	float error = 0.;
	int y = y0;

	// let's draw
	for (int x = x0; x <= x1; x++) {
		
		// we replaced x and y coordinates
		if (steep) {
			// now it's fine
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}

		error = error + derror;
		
		// each time error is greater than one pixel,
		// we increase (or decrease) y by one,
		// and decrease the error by one as well
		if (error > 0.5) {
			y = y + (y1 > y0 ? 1 : -1);
			error = error - 1.;
		}
	}

}

void line_5a(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
	
	bool steep = false;

	int width  = std::abs(x0 - x1);
	int height = std::abs(y0 - y1);

	// height > width, the line is steep
	// the swaps make sure the line is no more steep
	if (height > width) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	// x0 should always preceed x1
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0; // width
	int dy = y1 - y0; // height

	int derror = std::abs(dy) * 2;
	int error = 0;
	int y = y0;
	const int yincr = (y1 > y0? 1 : -1);

	// let's draw
	for (int x = x0; x <= x1; x++) {
		
		// we replaced x and y coordinates
		if (steep) {
			// now it's fine
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}

		error = error + derror;
		
		// each time error is greater than one pixel,
		// we increase (or decrease) y by one,
		// and decrease the error by one as well
		if (error > dx) {
			y = y + yincr;
			error = error - (dx * 2);
		}
	}

}
