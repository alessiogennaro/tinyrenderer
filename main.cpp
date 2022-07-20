
#include <cmath>
#include <utility>

#include "constants.h"
#include "tgaimage.h"
#include "line.h"
#include "lesson1.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


int main(int argc, char** argv) {
	
	TGAImage image(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);
	// image.set(52, 41, red);

	/* -------------------------------------------------------------- */

	// TODO: add boundary checks to final version of line fun
	line_5a(13, 20, 80, 40, image, white); 
	line_5a(20, 13, 40, 80, image, red); 
	line_5a(80, 40, 13, 20, image, red);
	
	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the image
	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;
}
