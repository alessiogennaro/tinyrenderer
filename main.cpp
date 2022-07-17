
#include <cmath>
#include <iostream>
#include <utility>

#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

/**
 * First attempt at drawing a line between (x0, y0) and (x1, y1)
 */
void line_fa(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (float t = 0.; t < 1.; t = t + 0.01) { // Problem: why 0.01?
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;

		image.set(x, y, color);
	}
}

/**
 * First attempt at drawing a line between (x0, y0) and (x1, y1).
 * Now we defined the value of step.
 * We need to know the number of pixels to draw, that is the number of
 * pixels between the two points
 */
void line_fa(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color, float step = 0.01) {
	for (float t = 0.; t < 1.; t = t + step) {
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;

		image.set(x, y, color);
	}
}

/**
 * Second attempt at drawing a line between (x0, y0) and (x1, y1).
 * Doesn't work for steep lines
 */
void line_sa(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float) (x1 - x0);
		int y = y0 * (1. - t) + y1 * t;

		image.set(x, y, color);
	}
}

void line_ta(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	
	bool steep = false;

	int width = std::abs(x0 - x1);
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
		float t = (x - x0) / (float) (x1 - x0);
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


int main(int argc, char** argv) {
	
	TGAImage image(100, 100, TGAImage::RGB);
	// image.set(52, 41, red);

	/* -------------------------------------------------------------- */

	line_ta(13, 20, 80, 40, image, white); 
	line_ta(20, 13, 40, 80, image, red); 
	line_ta(80, 40, 13, 20, image, red);
	
	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the image
	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;
}
