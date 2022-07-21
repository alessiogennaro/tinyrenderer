
#ifndef __LESSON1_H__
#define __LESSON1_H__

#include "../tgaimage.h"

/**
 * First attempt at drawing a line between (x0, y0) and (x1, y1)
 */
void line_1a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

/**
 * First attempt at drawing a line between (x0, y0) and (x1, y1).
 * Now we defined the value of step.
 * We need to know the number of pixels to draw, that is the number of
 * pixels between the two points
 */
void line_1a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color, float step = 0.01);

/**
 * Second attempt at drawing a line between (x0, y0) and (x1, y1).
 * Doesn't work for steep lines
 */
void line_2a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

/**
 * Third attempt. Now the three lines are displayed correctly.
 * Test:
 *  line_ta(13, 20, 80, 40, image, white); 
 *  line_ta(20, 13, 40, 80, image, red); 
 *  line_ta(80, 40, 13, 20, image, red);
 */
void line_3a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

/**
 * Fourth attempt
 */
void line_4a(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

/**
 * Fifth attempt. Now we get rid of floating points variables
 */
void line_5a(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

#endif
