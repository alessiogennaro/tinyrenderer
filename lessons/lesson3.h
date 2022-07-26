
#ifndef __LESSON3_H__
#define __LESSON3_H__

#include "../tgaimage.h"

/*
  A y-buffer example:

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


void rasterize(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color, int y_buffer[]);

#endif // __LESSON3_H__
