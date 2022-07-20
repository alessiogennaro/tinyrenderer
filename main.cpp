
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "constants.h"
#include "geometry.h"
#include "line.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"
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

	// wireframe_renderer(model.get(), image);
	
	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
	triangle(t0[0], t0[1], t0[2], image, red); 
	triangle(t1[0], t1[1], t1[2], image, white); 
	triangle(t2[0], t2[1], t2[2], image, green);

	/* -------------------------------------------------------------- */

	// i want to have the origin at the left bottom corner of the image
	image.flip_vertically();
	image.write_tga_file("output.tga");
	
	return 0;
}
