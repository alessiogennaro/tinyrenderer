
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"

#include "wireframe_renderer.h"


std::unique_ptr<Model> model {};


int main(int argc, char** argv) {
	
	if (argc == 2) {
		model = std::make_unique<Model>(argv[1]);
	} else {
		model = std::make_unique<Model>("obj/african_head.obj");
	}

	TGAImage image(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

	/* -------------------------------------------------------------- */

	wireframe_renderer(model.get(), image);
	
	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the image
	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;
}
