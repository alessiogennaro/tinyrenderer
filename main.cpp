
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
#include "math_constants.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"
#include "zbuffer_renderer.h"


std::unique_ptr<Model> model {};
Vec3f light_vec(0, 0, -1);


int main(int argc, char** argv) {
	
	if (argc == 3) {
		model = std::make_unique<Model>(argv[1]);
	} else {
		model = std::make_unique<Model>("obj/african_head.obj");
	}

	TGAImage canvas(CANVAS_WIDTH, CANVAS_HEIGHT, TGAImage::RGB);

	/* -------------------------------------------------------------- */

	zbuffer_renderer(model.get(), canvas, light_vec);

	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the canvas
	canvas.flip_vertically();
	canvas.write_tga_file("output.tga");
	
	return 0;
}
